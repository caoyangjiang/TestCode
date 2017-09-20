// Copyright 2017 Caoyang Jiang

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Aws/core/utils/json/JsonSerializer.h"

class Serializable
{
 public:
  Serializable()
  {
  }

  virtual ~Serializable()
  {
  }

  /**
   * @brief      Serialize into byte sequence
   *
   * @return     byte sequence
   */
  std::vector<uint8_t> Serialize()
  {
    Add_Values();
    return String_To_Vector(json_value_.WriteCompact());
  }

  /**
   * @brief      Deserialize data
   *
   * @param[in]  data  The data
   */
  void Deserialize(const std::vector<uint8_t>& data)
  {
    json_value_ = Aws::Utils::Json::JsonValue(Vector_To_String(data));

    if (!json_value_.WasParseSuccessful())
    {
      throw std::runtime_error("Deserialization parsing failed.");
    }

    Get_Values();
  }

 protected:
  virtual void Add_Values()
  {
  }

  virtual void Get_Values()
  {
  }

  void Add(const std::string& key, int value)
  {
    json_value_.WithInteger(Aws::String(key.c_str()), value);
  }

  void Add(const std::string& key, float value)
  {
    json_value_.WithDouble(Aws::String(key.c_str()),
                           static_cast<double>(value));
  }

  void Add(const std::string& key, double value)
  {
    json_value_.WithDouble(Aws::String(key.c_str()),
                           static_cast<double>(value));
  }

  void Add(const std::string& key, const std::string& value)
  {
    json_value_.WithString(Aws::String(key.c_str()),
                           Aws::String(value.c_str()));
  }

  void Add(const std::string& key, const std::vector<int>& value)
  {
    Aws::Vector<Aws::Utils::Json::JsonValue> array_json_value;

    for (auto& element : value)
    {
      Aws::Utils::Json::JsonValue json;
      json.AsInteger(element);
      array_json_value.push_back(json);
    }

    json_value_.WithArray(
        Aws::String(key.c_str()),
        Aws::Utils::Array<Aws::Utils::Json::JsonValue>(
            array_json_value.data(), array_json_value.size()));
  }

  void Add(const std::string& key, const std::vector<float>& value)
  {
    Aws::Vector<Aws::Utils::Json::JsonValue> array_json_value;
    for (auto& element : value)
    {
      Aws::Utils::Json::JsonValue json;
      json.AsDouble(static_cast<double>(element));
      array_json_value.push_back(json);
    }

    json_value_.WithArray(
        Aws::String(key.c_str()),
        Aws::Utils::Array<Aws::Utils::Json::JsonValue>(
            array_json_value.data(), array_json_value.size()));
  }

  void Add(const std::string& key, const std::vector<double>& value)
  {
    Aws::Vector<Aws::Utils::Json::JsonValue> array_json_value;
    for (auto& element : value)
    {
      Aws::Utils::Json::JsonValue json;
      json.AsDouble(element);
      array_json_value.push_back(json);
    }

    json_value_.WithArray(
        Aws::String(key.c_str()),
        Aws::Utils::Array<Aws::Utils::Json::JsonValue>(
            array_json_value.data(), array_json_value.size()));
  }

  void Add(const std::string& key, Serializable& serializable)
  {
    serializable.Add_Values();
    json_value_.WithObject(Aws::String(key.c_str()), serializable.json_value_);
  }

  void Get(const std::string& key, int& value)
  {
    if (!json_value_.ValueExists(Aws::String(key.c_str())))
    {
      throw std::runtime_error("key do not exist");
    }

    value = json_value_.GetInteger(Aws::String(key.c_str()));
  }

  void Get(const std::string& key, float& value)
  {
    if (!json_value_.ValueExists(Aws::String(key.c_str())))
    {
      throw std::runtime_error("key do not exist");
    }

    value = static_cast<float>(json_value_.GetDouble(Aws::String(key.c_str())));
  }

  void Get(const std::string& key, double& value)
  {
    if (!json_value_.ValueExists(Aws::String(key.c_str())))
    {
      throw std::runtime_error("key do not exist");
    }

    value = json_value_.GetDouble(Aws::String(key.c_str()));
  }

  void Get(const std::string& key, std::string& value)
  {
    if (!json_value_.ValueExists(Aws::String(key.c_str())))
    {
      throw std::runtime_error("key do not exist");
    }

    Aws::String temp;
    temp  = json_value_.GetString(Aws::String(key.c_str()));
    value = std::string(temp.c_str());
  }

  void Get(const std::string& key, std::vector<int>& value)
  {
    if (!json_value_.ValueExists(Aws::String(key.c_str())))
    {
      throw std::runtime_error("key do not exist");
    }

    Aws::Utils::Array<Aws::Utils::Json::JsonValue> array =
        json_value_.GetArray(Aws::String(key.c_str()));

    for (size_t i = 0; i < array.GetLength(); i++)
    {
      value.push_back(array[i].AsInteger());
    }
  }

  void Get(const std::string& key, std::vector<double>& value)
  {
    if (!json_value_.ValueExists(Aws::String(key.c_str())))
    {
      throw std::runtime_error("key do not exist");
    }

    Aws::Utils::Array<Aws::Utils::Json::JsonValue> array =
        json_value_.GetArray(Aws::String(key.c_str()));

    for (size_t i = 0; i < array.GetLength(); i++)
    {
      value.push_back(array[i].AsDouble());
    }
  }

  void Get(const std::string& key, std::vector<float>& value)
  {
    if (!json_value_.ValueExists(Aws::String(key.c_str())))
    {
      throw std::runtime_error("key do not exist");
    }

    Aws::Utils::Array<Aws::Utils::Json::JsonValue> array =
        json_value_.GetArray(Aws::String(key.c_str()));

    for (size_t i = 0; i < array.GetLength(); i++)
    {
      value.push_back(static_cast<float>(array[i].AsDouble()));
    }
  }

  void Get(const std::string& key, Serializable& value)
  {
    if (!json_value_.ValueExists(Aws::String(key.c_str())))
    {
      throw std::runtime_error("key do not exist");
    }

    value.json_value_ = json_value_.GetObject(Aws::String(key.c_str()));
    value.Get_Values();
  }

 private:
  Aws::String Vector_To_String(const std::vector<uint8_t>& vec) const
  {
    Aws::String str;

    for (size_t i = 0; i < vec.size(); i++)
    {
      str.append(1, static_cast<char>(vec[i]));
    }

    return str;
  }

  std::vector<uint8_t> String_To_Vector(const Aws::String& str) const
  {
    std::vector<uint8_t> vec;

    for (size_t i = 0; i < str.size(); i++)
    {
      vec.push_back(str[i]);
    }

    // vector representation will have the null-terminator
    vec.push_back(0x00);

    return vec;
  }

 private:
  Aws::Utils::Json::JsonValue json_value_;
};

struct Mode : public Serializable
{
  struct SubMode : public Serializable
  {
    int subheight = 0;
    int subwidth  = 0;

    ~SubMode() override
    {
    }

    void Add_Values() override
    {
      Add("subheight", subheight);
      Add("subwidth", subwidth);
    }

    void Get_Values() override
    {
      Get("subheight", subheight);
      Get("subwidth", subwidth);
    }
  };

  int width              = 2048;
  int height             = 1024;
  int mode               = 0;
  float ratio            = 0.91223129;
  double high_ratio      = 0.1234567;
  std::vector<int> nums  = {1, 2, 3, 4, 5};
  std::vector<double> ds = {0.123, 0.321, 112.2};
  SubMode sub_modes;

  Mode()
  {
    sub_modes.subheight = 512;
    sub_modes.subwidth  = 256;
  }

  explicit Mode(bool using_default)
  {
    if (!using_default)
    {
      width      = 0;
      height     = 0;
      mode       = 0;
      ratio      = 0;
      high_ratio = 0;
      nums       = std::vector<int>();
      ds         = std::vector<double>();
    }
  }

  ~Mode() override
  {
  }

  void Add_Values() override
  {
    Add("width", width);
    Add("height", height);
    Add("mode", mode);
    Add("high_ratio", high_ratio);
    Add("ratio", ratio);
    Add("nums", nums);
    Add("ds", ds);
    Add("sub_modes", sub_modes);
  }

  void Get_Values() override
  {
    Get("width", width);
    Get("height", height);
    Get("mode", mode);
    Get("high_ratio", high_ratio);
    Get("ratio", ratio);
    Get("nums", nums);
    Get("ds", ds);
    Get("sub_modes", sub_modes);
  }

  void Print()
  {
    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;
    std::cout << "mode: " << mode << std::endl;
    std::cout << "ratio: " << ratio << std::endl;
    std::cout << "high_ratio: " << high_ratio << std::endl;
    for (auto& value : nums)
    {
      std::cout << value << " ";
    }
    std::cout << std::endl;

    for (auto& value : ds)
    {
      std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "sub_modes:subheight" << sub_modes.subheight << std::endl;
    std::cout << "sub_modes:subwidth" << sub_modes.subwidth << std::endl;
  }
};

int main()
{
  Mode mode_data[2];

  mode_data[1] = Mode(false);
  std::cout << "Before" << std::endl;
  mode_data[1].Print();

  std::vector<uint8_t> bytes;
  bytes = mode_data[0].Serialize();
  std::cout << bytes.size() << std::endl;
  mode_data[1].Deserialize(bytes);

  std::cout << "After " << std::endl;
  mode_data[1].Print();
}
