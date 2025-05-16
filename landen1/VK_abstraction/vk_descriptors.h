#pragma once

#include "vk_device.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>


namespace vkc {
    class VkcDescriptorSetLayout:
    virtual public ::particle{
    public:
        class Builder  {
        public:
            Builder(VkcDevice *pvkcDevice) : m_pvkcdevice{ pvkcDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            ::pointer<VkcDescriptorSetLayout> build() const;

        private:
            ::pointer < VkcDevice > m_pvkcdevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        VkcDescriptorSetLayout(
            VkcDevice * pvkcdevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~VkcDescriptorSetLayout();
        VkcDescriptorSetLayout(const VkcDescriptorSetLayout&) = delete;
        VkcDescriptorSetLayout& operator=(const VkcDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        ::pointer < VkcDevice > m_pvkcdevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class VkcDescriptorWriter;
    };

    class VkcDescriptorPool :
    virtual public ::particle {
    public:
        class Builder :
        virtual public ::particle{
        public:
            //Builder(VkcDevice& m_pvkcdevice) : m_pvkcdevice{ m_pvkcdevice } {}
            Builder() {}
            void initialize_builder(VkcDevice* pvkcDevice)
            {
               m_pvkcdevice = pvkcDevice;

            }
            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            ::pointer<VkcDescriptorPool> build() const;

        private:
            ::pointer < VkcDevice > m_pvkcdevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        VkcDescriptorPool(
            VkcDevice * pvkcdevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~VkcDescriptorPool();
        VkcDescriptorPool(const VkcDescriptorPool&) = delete;
        VkcDescriptorPool& operator=(const VkcDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        ::pointer < VkcDevice > m_pvkcdevice;
        VkDescriptorPool descriptorPool;

        friend class VkcDescriptorWriter;
    };

    class VkcDescriptorWriter {
    public:
        VkcDescriptorWriter(VkcDescriptorSetLayout& setLayout, VkcDescriptorPool& pool);

        VkcDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VkcDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        VkcDescriptorSetLayout& setLayout;
        VkcDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}