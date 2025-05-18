#pragma once


#include "app-cube/cube/application_object.h"


//
//#include "model.h"
//
//#include "acme/prototype/collection/map.h"
//// libs
//#include <glm/gtc/matrix_transform.hpp>
//
//// std
//#include <memory>
//#include <unordered_map>
//
//
//namespace graphics3d_vulkan
//{
//
//
//	struct TransformComponent {
//		glm::vec3 translation{};
//		glm::vec3 scale{ 1.f, 1.f, 1.f };
//		glm::vec3 rotation{};
//
//		// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
//		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
//
//		glm::mat4 mat4();
//		glm::mat3 normalMatrix();
//
//	};
//
//	struct PointLightComponent :
//	virtual public ::particle{
//		float lightIntensity = 1.0f;
//	};
//
//
//	class application_object :
//		virtual public ::quantum
//	{
//	public:
//		
//		::collection::index m_iId;
//
//		TransformComponent m_transform;
//
//		glm::vec3 m_color;
//
//		::pointer<model> m_pmodel;
//
//		using map = index_map < ::pointer < application_object > >;
//
//		static interlocked_long_long s_interlockedll;
//		application_object() : 
//			m_iId{ s_interlockedll++ }
//		{
//		
//		}
//
//	};
//
//
//	class point_light :
//		virtual public application_object
//	{
//	public:
//
//		
//		PointLightComponent	m_pointlightcomponent;
//
//
//		point_light(float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));
//		~point_light() override;
//
//	};
//
//
//} // graphics3d_vulkan
//
//
//
