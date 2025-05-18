#pragma once


#include "context.h"


// std
#include <memory>
#include <unordered_map>
#include <vector>


namespace graphics3d_vulkan 
{


    class CLASS_DECL_GRAPHICS3D_VULKAN set_descriptor_layout:
      virtual public ::particle
    {
    public:
        class Builder  
        {
        public:
            Builder(context *pvkcDevice) : m_pcontext{ pvkcDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            ::pointer<set_descriptor_layout> build() const;

        private:
            ::pointer < context > m_pcontext;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        set_descriptor_layout(
            context * pvkcdevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~set_descriptor_layout();
        set_descriptor_layout(const set_descriptor_layout&) = delete;
        set_descriptor_layout& operator=(const set_descriptor_layout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        ::pointer < context > m_pcontext;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class descriptor_writer;
    };

    class CLASS_DECL_GRAPHICS3D_VULKAN descriptor_pool :
    virtual public ::particle {
    public:
        class Builder :
        virtual public ::particle{
        public:
            //Builder(context& m_pcontext) : m_pcontext{ m_pcontext } {}
            Builder() {}
            void initialize_builder(context* pvkcDevice)
            {
               m_pcontext = pvkcDevice;

            }
            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            ::pointer<descriptor_pool> build() const;

        private:
            ::pointer < context > m_pcontext;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        descriptor_pool(
            context * pvkcdevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~descriptor_pool();
        descriptor_pool(const descriptor_pool&) = delete;
        descriptor_pool& operator=(const descriptor_pool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        ::pointer < context > m_pcontext;
        VkDescriptorPool descriptorPool;

        friend class descriptor_writer;
    };

    
    class CLASS_DECL_GRAPHICS3D_VULKAN descriptor_writer
    {
    public:
        descriptor_writer(set_descriptor_layout& setLayout, descriptor_pool& pool);

        descriptor_writer& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        descriptor_writer& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        set_descriptor_layout& setLayout;
        descriptor_pool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };


} // namespace graphics3d_vulkan



