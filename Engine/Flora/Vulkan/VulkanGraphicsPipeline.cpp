#include "VulkanGraphicsPipeline.hpp"

namespace FloraEngine {
VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanDevice *device)
    : pVulkanDeviceHandle(device) {}
VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {}

static std::vector<char> readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file!");
  }

  size_t            file_size = (size_t)file.tellg();
  std::vector<char> buffer(file_size);

  file.seekg(0);
  file.read(buffer.data(), file_size);
  file.close();

  return buffer;
}

void VulkanGraphicsPipeline::Init() {
  UpdateShaderModules();

  /* Create vertex shader stage */
  VkPipelineShaderStageCreateInfo vertex_shader_stage_info{};
  vertex_shader_stage_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertex_shader_stage_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  vertex_shader_stage_info.module = mVertexShaderModule;
  vertex_shader_stage_info.pName  = "main";

  /* Create fragment shader stage */
  VkPipelineShaderStageCreateInfo fragment_shader_stage_info{};
  fragment_shader_stage_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragment_shader_stage_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragment_shader_stage_info.module = mFragmentShaderModule;
  fragment_shader_stage_info.pName  = "main";

  VkPipelineShaderStageCreateInfo shader_stages[] = {
      vertex_shader_stage_info,
      fragment_shader_stage_info};
}

void VulkanGraphicsPipeline::Cleanup() {
  vkDestroyShaderModule(pVulkanDeviceHandle->GetDevice(),
                        mVertexShaderModule,
                        nullptr);
  vkDestroyShaderModule(pVulkanDeviceHandle->GetDevice(),
                        mFragmentShaderModule,
                        nullptr);
}

void VulkanGraphicsPipeline::UpdateShaderModules() {
  // TODO: Destroy vulkan objects before recreating
  CompileGLSLtoSPIRV();
  auto vertex_shader_code   = readFile("./Engine/Shaders/vertex.spv");
  auto fragment_shader_code = readFile("./Engine/Shaders/fragment.spv");

  /* Create vertex shader module*/
  VkShaderModuleCreateInfo create_info{};
  create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = vertex_shader_code.size();
  create_info.pCode =
      reinterpret_cast<const uint32_t *>(vertex_shader_code.data());

  if (vkCreateShaderModule(pVulkanDeviceHandle->GetDevice(),
                           &create_info,
                           nullptr,
                           &mVertexShaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create vertex shader module!");
  }

  /* Create fragment shader module*/
  create_info.codeSize = fragment_shader_code.size();
  create_info.pCode =
      reinterpret_cast<const uint32_t *>(fragment_shader_code.data());

  if (vkCreateShaderModule(pVulkanDeviceHandle->GetDevice(),
                           &create_info,
                           nullptr,
                           &mFragmentShaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create fragment shader module!");
  }
}

void VulkanGraphicsPipeline::CompileGLSLtoSPIRV() {
  std::system("glslc "
              "./Engine/Shaders/shader.vert "
              "-o ./Engine/Shaders/vertex.spv");
  std::system("glslc "
              "./Engine/Shaders/shader.frag "
              "-o ./Engine/Shaders/fragment.spv");
}
} // namespace FloraEngine