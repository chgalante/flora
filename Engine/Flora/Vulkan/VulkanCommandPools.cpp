#include "VulkanCommandPools.hpp"

namespace FloraEngine {
VulkanCommandPools::VulkanCommandPools(VulkanDevice           *device,
                                       VulkanSwapChain        *swap_chain,
                                       VulkanGraphicsPipeline *pipeline,
                                       VulkanFramebuffers     *framebuffers)
    : pVulkanDeviceHandle(device), pVulkanSwapChainHandle(swap_chain),
      pVulkanGraphicsPipelineHandle(pipeline),
      pVulkanFramebuffersHandle(framebuffers) {}
VulkanCommandPools::~VulkanCommandPools() {}

void VulkanCommandPools::Init() {
  const QueueFamilyIndices &queue_family_indices =
      pVulkanDeviceHandle->GetQueueFamilyIndices();

  VkCommandPoolCreateInfo command_pool_info{};
  command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  command_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  command_pool_info.queueFamilyIndex =
      queue_family_indices.graphicsFamily.value();

  if (vkCreateCommandPool(pVulkanDeviceHandle->GetDevice(),
                          &command_pool_info,
                          nullptr,
                          &mCommandPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }

  VkCommandBufferAllocateInfo command_buffer_allocate_info{};
  command_buffer_allocate_info.sType =
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  command_buffer_allocate_info.commandPool = mCommandPool;
  command_buffer_allocate_info.level       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_allocate_info.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(pVulkanDeviceHandle->GetDevice(),
                               &command_buffer_allocate_info,
                               &mCommandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void VulkanCommandPools::Cleanup() {
  vkDestroyCommandPool(pVulkanDeviceHandle->GetDevice(), mCommandPool, nullptr);
}

void VulkanCommandPools::RecordCommandBuffer(VkCommandBuffer command_buffer,
                                             uint32_t        image_index) {
  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags            = 0;       // Optional
  begin_info.pInheritanceInfo = nullptr; // Optional

  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  const std::vector<VkFramebuffer> &framebuffers =
      pVulkanFramebuffersHandle->GetFramebuffers();

  VkRenderPassBeginInfo render_pass_info{};
  render_pass_info.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_info.renderPass  = pVulkanGraphicsPipelineHandle->GetRenderPass();
  render_pass_info.framebuffer = framebuffers[image_index];
  render_pass_info.renderArea.offset = {0, 0};
  render_pass_info.renderArea.extent =
      pVulkanSwapChainHandle->GetSwapChainExtent2D();

  VkClearValue clear_color         = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues    = &clear_color;

  vkCmdBeginRenderPass(mCommandBuffer,
                       &render_pass_info,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(mCommandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pVulkanGraphicsPipelineHandle->GetGraphicsPipeline());
  vkCmdDraw(mCommandBuffer, 3, 1, 0, 0);
  vkCmdEndRenderPass(mCommandBuffer);

  if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}
} // namespace FloraEngine