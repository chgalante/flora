#include "VulkanGraphicsPipeline.hpp"

namespace FloraEngine {
VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanDevice    *device,
                                               VulkanSwapChain *swap_chain)
    : pVulkanDeviceHandle(device), pVulkanSwapChainHandle(swap_chain) {}
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
  /* Compile glsl shaders to spirv*/
  compile_glsl_to_spirv();
  auto vertex_shader_code   = readFile("../Engine/Shaders/vertex.spv");
  auto fragment_shader_code = readFile("../Engine/Shaders/fragment.spv");

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

  /* vertex shader stage */
  VkPipelineShaderStageCreateInfo vertex_shader_stage_info{};
  vertex_shader_stage_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertex_shader_stage_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  vertex_shader_stage_info.module = mVertexShaderModule;
  vertex_shader_stage_info.pName  = "main";

  /* fragment shader stage */
  VkPipelineShaderStageCreateInfo fragment_shader_stage_info{};
  fragment_shader_stage_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragment_shader_stage_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragment_shader_stage_info.module = mFragmentShaderModule;
  fragment_shader_stage_info.pName  = "main";

  /* shader stages */
  VkPipelineShaderStageCreateInfo shader_stages[] = {
      vertex_shader_stage_info,
      fragment_shader_stage_info};

  /* vertex input state info */
  VkPipelineVertexInputStateCreateInfo vertex_input_info{};
  vertex_input_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_info.vertexBindingDescriptionCount   = 0;
  vertex_input_info.pVertexBindingDescriptions      = nullptr; // Optional
  vertex_input_info.vertexAttributeDescriptionCount = 0;
  vertex_input_info.pVertexAttributeDescriptions    = nullptr; // Optional

  /* input assembly state info */
  VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
  input_assembly_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_info.primitiveRestartEnable = VK_FALSE;

  /* viewport state info */
  mViewport       = VkViewport{};
  mViewport.x     = 0.0f;
  mViewport.y     = 0.0f;
  mViewport.width = (float)pVulkanSwapChainHandle->GetSwapChainExtent2D().width;
  mViewport.height =
      (float)pVulkanSwapChainHandle->GetSwapChainExtent2D().height;
  mViewport.minDepth = 0.0f;
  mViewport.maxDepth = 1.0f;

  mScissor        = VkRect2D{};
  mScissor.offset = {0, 0};
  mScissor.extent = pVulkanSwapChainHandle->GetSwapChainExtent2D();

  VkPipelineViewportStateCreateInfo viewport_state_info{};
  viewport_state_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_info.viewportCount = 1;
  viewport_state_info.pViewports    = &mViewport;
  viewport_state_info.scissorCount  = 1;
  viewport_state_info.pScissors     = &mScissor;

  /* rasterization state info */
  VkPipelineRasterizationStateCreateInfo rasterization_state_info{};
  rasterization_state_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterization_state_info.depthClampEnable        = VK_FALSE;
  rasterization_state_info.rasterizerDiscardEnable = VK_FALSE;
  rasterization_state_info.polygonMode             = VK_POLYGON_MODE_FILL;
  rasterization_state_info.lineWidth               = 1.0f;
  rasterization_state_info.cullMode                = VK_CULL_MODE_BACK_BIT;
  rasterization_state_info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
  rasterization_state_info.depthBiasEnable         = VK_FALSE;
  rasterization_state_info.depthBiasConstantFactor = 0.0f; // Optional
  rasterization_state_info.depthBiasClamp          = 0.0f; // Optional
  rasterization_state_info.depthBiasSlopeFactor    = 0.0f; // Optional

  /* multisample state info
   * Multisampling is disabled for now.
   */
  VkPipelineMultisampleStateCreateInfo multisample_state_info{};
  multisample_state_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample_state_info.sampleShadingEnable   = VK_FALSE;
  multisample_state_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
  multisample_state_info.minSampleShading      = 1.0f;     // Optional
  multisample_state_info.pSampleMask           = nullptr;  // Optional
  multisample_state_info.alphaToCoverageEnable = VK_FALSE; // Optional
  multisample_state_info.alphaToOneEnable      = VK_FALSE; // Optional

  /* color blend attachment state
   * Color blending is disabled for now.
   * Find all possible operations in the VkBlendFactor and VkBlendOp enums.
   * It is possible to blend using bitwise combination (logicOpEnable, logicOp).
   */
  VkPipelineColorBlendAttachmentState color_blend_attachment{};
  color_blend_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment.blendEnable         = VK_FALSE;
  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  color_blend_attachment.colorBlendOp        = VK_BLEND_OP_ADD;      // Optional
  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  color_blend_attachment.alphaBlendOp        = VK_BLEND_OP_ADD;      // Optional

  VkPipelineColorBlendStateCreateInfo color_blend_state_info{};
  color_blend_state_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blend_state_info.logicOpEnable     = VK_FALSE;
  color_blend_state_info.logicOp           = VK_LOGIC_OP_COPY; // Optional
  color_blend_state_info.attachmentCount   = 1;
  color_blend_state_info.pAttachments      = &color_blend_attachment;
  color_blend_state_info.blendConstants[0] = 0.0f; // Optional
  color_blend_state_info.blendConstants[1] = 0.0f; // Optional
  color_blend_state_info.blendConstants[2] = 0.0f; // Optional
  color_blend_state_info.blendConstants[3] = 0.0f; // Optional

  /* choose dynamic states
   * States that are dynamic must be specified at draw time.
   * For these states, configuration values are ignored.
   */
  VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                     VK_DYNAMIC_STATE_LINE_WIDTH};

  VkPipelineDynamicStateCreateInfo dynamic_state_info{};
  dynamic_state_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state_info.dynamicStateCount = 2;
  dynamic_state_info.pDynamicStates    = dynamic_states;

  /* pipeline layout */
  VkPipelineLayoutCreateInfo layout_info{};
  layout_info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  layout_info.setLayoutCount = 0;               // Optional
  layout_info.pSetLayouts    = nullptr;         // Optional
  layout_info.pushConstantRangeCount = 0;       // Optional
  layout_info.pPushConstantRanges    = nullptr; // Optional

  if (vkCreatePipelineLayout(pVulkanDeviceHandle->GetDevice(),
                             &layout_info,
                             nullptr,
                             &mLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }

  /* render pass */
  VkAttachmentDescription color_attachment{};
  color_attachment.format  = pVulkanSwapChainHandle->GetSwapChainImageFormat();
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref{};
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments    = &color_attachment_ref;

  VkRenderPassCreateInfo render_pass_info{};
  render_pass_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments    = &color_attachment;
  render_pass_info.subpassCount    = 1;
  render_pass_info.pSubpasses      = &subpass;

  if (vkCreateRenderPass(pVulkanDeviceHandle->GetDevice(),
                         &render_pass_info,
                         nullptr,
                         &mRenderPass) != VK_SUCCESS) {
    throw std::runtime_error("failed to create render pass!");
  }

  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.stageCount = 2;
  pipeline_info.pStages    = shader_stages;
  pipeline_info.pVertexInputState   = &vertex_input_info;
  pipeline_info.pInputAssemblyState = &input_assembly_info;
  pipeline_info.pViewportState      = &viewport_state_info;
  pipeline_info.pRasterizationState = &rasterization_state_info;
  pipeline_info.pMultisampleState   = &multisample_state_info;
  pipeline_info.pDepthStencilState  = nullptr; // Optional
  pipeline_info.pColorBlendState    = &color_blend_state_info;
  pipeline_info.pDynamicState       = nullptr; // Optional
  pipeline_info.layout              = mLayout;
  pipeline_info.renderPass          = mRenderPass;
  pipeline_info.subpass             = 0;
  pipeline_info.basePipelineHandle  = VK_NULL_HANDLE; // Optional
  pipeline_info.basePipelineIndex   = -1;             // Optional

  if (vkCreateGraphicsPipelines(pVulkanDeviceHandle->GetDevice(),
                                VK_NULL_HANDLE,
                                1,
                                &pipeline_info,
                                nullptr,
                                &mGraphicsPipeline) != VK_SUCCESS) {
    throw std::runtime_error("failed to create graphics pipeline!");
  }
}

void VulkanGraphicsPipeline::Cleanup() {
  vkDestroyShaderModule(pVulkanDeviceHandle->GetDevice(),
                        mVertexShaderModule,
                        nullptr);
  vkDestroyShaderModule(pVulkanDeviceHandle->GetDevice(),
                        mFragmentShaderModule,
                        nullptr);
  vkDestroyPipeline(pVulkanDeviceHandle->GetDevice(),
                    mGraphicsPipeline,
                    nullptr);
  vkDestroyPipelineLayout(pVulkanDeviceHandle->GetDevice(), mLayout, nullptr);
  vkDestroyRenderPass(pVulkanDeviceHandle->GetDevice(), mRenderPass, nullptr);
}

void VulkanGraphicsPipeline::compile_glsl_to_spirv() {
  FE_CORE_TRACE("Compiling shaders...");
  std::system("pwd");
  std::system("glslc "
              "../Engine/Shaders/shader.vert "
              "-o ../Engine/Shaders/vertex.spv");
  std::system("glslc "
              "../Engine/Shaders/shader.frag "
              "-o ../Engine/Shaders/fragment.spv");
}
} // namespace FloraEngine