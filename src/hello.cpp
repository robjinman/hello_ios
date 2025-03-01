#include "hello.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

class Exception : public std::runtime_error
{
public:
  Exception(const std::string& msg, const std::string& file, int line)
    : runtime_error(msg + " (file: " + file + ", line: " + std::to_string(line) + ")") {}
};

#define EXCEPTION(msg) \
  { \
    std::stringstream MAC_ss; \
    MAC_ss << msg; \
    throw Exception(MAC_ss.str(), __FILE__, __LINE__); \
  }

#define VK_CHECK(fnCall, msg) \
  { \
    VkResult MAC_code = fnCall; \
    if (MAC_code != VK_SUCCESS) { \
      EXCEPTION(msg << " (result: " << MAC_code << ")"); \
    } \
  }

const std::vector<const char*> ValidationLayers = {
  "VK_LAYER_KHRONOS_validation"
};

void checkValidationLayerSupport()
{
  uint32_t layerCount;
  VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr),
    "Failed to enumerate instance layer properties");

  std::vector<VkLayerProperties> available(layerCount);
  VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, available.data()),
    "Failed to enumerate instance layer properties");

  for (auto layer : ValidationLayers) {
    auto fnMatches = [=](const VkLayerProperties& p) {
      return strcmp(layer, p.layerName) == 0;
    };
    if (std::find_if(available.begin(), available.end(), fnMatches) == available.end()) {
      EXCEPTION("Validation layer '" << layer << "' not supported");
    }
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT,
  const VkDebugUtilsMessengerCallbackDataEXT* data, void*)
{
  std::cerr << "Validation layer: " << data->pMessage << std::endl;

  return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo()
{
  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;
  return createInfo;
}

std::vector<const char*> getRequiredExtensions()
{
  std::vector<const char*> extensions;
  extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  return extensions;
}

void initialiseVulkan()
{
  checkValidationLayerSupport();

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Project Nova";
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  createInfo.enabledLayerCount = ValidationLayers.size();
  createInfo.ppEnabledLayerNames = ValidationLayers.data();

  auto debugMessengerInfo = getDebugMessengerCreateInfo();
  createInfo.pNext = &debugMessengerInfo;

  auto extensions = getRequiredExtensions();

  createInfo.enabledExtensionCount = extensions.size();
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkInstance instance;
  VK_CHECK(vkCreateInstance(&createInfo, nullptr, &instance), "Failed to create instance");
}

std::string get_hello_message()
{
  bool success = true;
  std::string reason;

  try {
    initialiseVulkan();
  }
  catch (const std::exception& e) {
    success = false;
    reason = e.what();
  }

  std::stringstream ss;
  ss << "Vulkan initialisation " << (success ? "succeeded" : "failed");
  if (!success) {
    ss << ". Reason: " << reason;
  }

  return ss.str();
}
