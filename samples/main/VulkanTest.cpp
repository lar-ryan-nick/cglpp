#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vector>
#include <iostream>
#include <cstring>

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
  std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

void populateDeviceQueueCreateInfo(uint32_t queueFamilyIndex, float* queuePriority, VkDeviceQueueCreateInfo& queueCreateInfo) {
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = queuePriority;
}

int main(int argc, char** argv) {
  glfwInit();

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkan Test";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  uint32_t availableExtensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
  std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

  uint32_t requiredExtensionCount = 0;
  const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
  std::vector<const char*> extensions(requiredExtensions, requiredExtensions + requiredExtensionCount);
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  uint32_t availableLayerCount;
  vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);
  std::vector<VkLayerProperties> availableLayers(availableLayerCount);
  vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());

  std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

  for (const char* validationLayer : validationLayers) {
    int j = 0;
    for (; j < availableLayers.size(); ++j) {
      if (strcmp(validationLayer, availableLayers[j].layerName) == 0) {
        break;
      }
    }
    if (j >= availableLayers.size()) {
      std::cerr << "Could not find validation layer: " << validationLayer << std::endl;
    }
  }

  VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
  debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debugMessengerCreateInfo.pfnUserCallback = debugCallback;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();
  createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
  createInfo.ppEnabledLayerNames = validationLayers.data();
  createInfo.pNext = &debugMessengerCreateInfo;

  VkInstance instance;
  VkResult instanceCreationResult = vkCreateInstance(&createInfo, nullptr, &instance);
  if (instanceCreationResult != VK_SUCCESS) {
    std::cerr << "Failed to create vulkan instance" << std::endl;
    return -1;
  }

  VkDebugUtilsMessengerEXT debugMessenger;
  auto createDebugMessenger = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (createDebugMessenger == nullptr) {
    std::cerr << "Failed to find debug messenger creation function" << std::endl;
    return -1;
  }
  VkResult debugUtilsCreationResult = createDebugMessenger(instance, &debugMessengerCreateInfo, nullptr, &debugMessenger);
  if (debugUtilsCreationResult != VK_SUCCESS) {
    std::cerr << "Failed to create vulkan debug messenger" << std::endl;
    return -1;
  }
  
  // Create window.
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(1600, 900, "Vulkan", nullptr, nullptr);

  // Create window surface.
  VkSurfaceKHR surface;
  VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
  surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  surfaceCreateInfo.hwnd = glfwGetWin32Window(window);
  surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);

  if (vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
    std::cerr << "failed to create window surface!" << std::endl;
    return -1;
  }

  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
    std::cerr << "failed to create window surface!" << std::endl;
  }

  // Choose physical device
  uint32_t availablePhysicalDeviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &availablePhysicalDeviceCount, nullptr);
  std::vector<VkPhysicalDevice> availablePhysicalDevices(availablePhysicalDeviceCount);
  vkEnumeratePhysicalDevices(instance, &availablePhysicalDeviceCount, availablePhysicalDevices.data());

  std::cout << availablePhysicalDeviceCount << " physical devices available" << std::endl;

  VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
  uint32_t graphicsQueueFamilyIndex;
  uint32_t presentationQueueFamilyIndex;

  for (const VkPhysicalDevice& physicalDevice : availablePhysicalDevices) {
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);
    if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && physicalDeviceFeatures.geometryShader) {
      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
      graphicsQueueFamilyIndex = presentationQueueFamilyIndex = queueFamilyCount;
      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
      for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        if (queueFamilies[graphicsQueueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          graphicsQueueFamilyIndex = i;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if (presentSupport) {
          presentationQueueFamilyIndex = i;
        }
      }
      if (graphicsQueueFamilyIndex < queueFamilyCount && presentationQueueFamilyIndex < queueFamilyCount) {
        std::cout << "Selected physical device " << physicalDeviceProperties.deviceName << std::endl;
        selectedPhysicalDevice = physicalDevice;
        break;
      }
    }
  }

  if (selectedPhysicalDevice == VK_NULL_HANDLE) {
    std::cerr << "Couldn't find a physical device" << std::endl;
    return -1;
  }

  // Create logical device.
  float queuePriority = 1.0f;
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(2, VkDeviceQueueCreateInfo{});
  populateDeviceQueueCreateInfo(graphicsQueueFamilyIndex, &queuePriority, queueCreateInfos[0]);
  populateDeviceQueueCreateInfo(presentationQueueFamilyIndex, &queuePriority, queueCreateInfos[1]);

  VkDeviceCreateInfo deviceCreateInfo{};
  VkPhysicalDeviceFeatures enabledPhysicalDeviceFeatures{};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
  deviceCreateInfo.pEnabledFeatures = &enabledPhysicalDeviceFeatures;
  createInfo.enabledExtensionCount = 0;
  createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
  createInfo.ppEnabledLayerNames = validationLayers.data();

  VkDevice device;
  if (vkCreateDevice(selectedPhysicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
    std::cerr << "failed to create logical device!" << std::endl;
    return -1;
  }

  VkQueue graphicsQueue;
  VkQueue presentationQueue;
  vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &graphicsQueue);
  vkGetDeviceQueue(device, presentationQueueFamilyIndex, 0, &presentationQueue);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
  vkDeviceWaitIdle(device);

  vkDestroyDevice(device, nullptr);

  // Cleanup.
  vkDestroySurfaceKHR(instance, surface, nullptr);

  glfwDestroyWindow(window);

  auto destroyDebugMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (destroyDebugMessenger != nullptr) {
    destroyDebugMessenger(instance, debugMessenger, nullptr);
  }

  vkDestroyInstance(instance, nullptr);

  glfwTerminate();
}
