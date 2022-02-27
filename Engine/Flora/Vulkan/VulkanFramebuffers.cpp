#include "VulkanFramebuffers.hpp"

namespace FloraEngine {
VulkanFramebuffers::VulkanFramebuffers(VulkanDevice           *device,
                                       VulkanSwapChain        *swap_chain,
                                       VulkanImageViews       *image_views,
                                       VulkanGraphicsPipeline *pipeline)
    : pVulkanDeviceHandle(device), pVulkanSwapChainHandle(swap_chain),
      pVulkanImageViewsHandle(image_views),
      pVulkanGraphicsPipelineHandle(pipeline) {}
VulkanFramebuffers::~VulkanFramebuffers() {}

void VulkanFramebuffers::Init() {
  const std::vector<VkImageView> &image_views =
      pVulkanImageViewsHandle->GetImageViews();
  const VkRenderPass &render_pass =
      pVulkanGraphicsPipelineHandle->GetRenderPass();
  const VkExtent2D &swap_chain_extent =
      pVulkanSwapChainHandle->GetSwapChainExtent2D();

  mFramebuffers.resize(image_views.size());

  for (size_t i = 0; i < image_views.size(); i++) {
    VkImageView attachments[] = {image_views[i]};

    VkFramebufferCreateInfo framebuffer_info{};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass =
        pVulkanGraphicsPipelineHandle->GetRenderPass();
    framebuffer_info.attachmentCount = 1;
    framebuffer_info.pAttachments    = attachments;
    framebuffer_info.width           = swap_chain_extent.width;
    framebuffer_info.height          = swap_chain_extent.height;
    framebuffer_info.layers          = 1;

    if (vkCreateFramebuffer(pVulkanDeviceHandle->GetDevice(),
                            &framebuffer_info,
                            nullptr,
                            &mFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

void VulkanFramebuffers::Cleanup() {
  for (auto framebuffer : mFramebuffers) {
    vkDestroyFramebuffer(pVulkanDeviceHandle->GetDevice(),
                         framebuffer,
                         nullptr);
  }
}

std::vector<VkFramebuffer> VulkanFramebuffers::GetFramebuffers() {
  return mFramebuffers;
}
} // namespace FloraEngine
