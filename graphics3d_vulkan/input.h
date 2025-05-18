#pragma once 


#include "app-cube/cube/graphics3d/input.h"


//// MNK_Controller.h
//#include "scene_object.h"
////#include "window.h"
//#include "acme/constant/user_key.h"
//#include "app-cube/cube/graphics3d/key_map.h"
//#include <glm/glm.hpp>
//
//
//namespace graphics3d_vulkan 
//{
//
//
//   struct input_t
//   {
//      //KeyMappings keys{};
//      float moveSpeed{ 3.f };
//      float lookSpeed{ 1.5f };
//      bool   m_bMouseAbsolute;
//
//      double m_dMouseLastX = 0.0;
//      double m_dMouseLastY = 0.0;
//      double m_Δx = 0.;
//      double m_Δy = 0.;
//
//
//
//
//      float _sensitivity;  // Sensitivity factor for mouse movement
//      float _yaw;          // Camera yaw (horizontal rotation)
//      float _pitch;        // Camera pitch (vertical rotation)
//      glm::vec3 _cameraDirection; // Camera direction vector
//      glm::vec3 _cameraPosition;
//
//      ::block as_block()
//      {
//
//         return as_memory_block(*this);
//
//      }
//
//   };
//
//   class CLASS_DECL_GRAPHICS3D_VULKAN input :
//      virtual public input_t
//   {
//   public:
//
//       
//
//      MNKController( float sensitivity = 0.1f, float yaw = -90.0f, float pitch = 0.0f);
//
//      //void moveInPlaneXZ(::cube::impact * pimpact, float dt, application_object& gameObject);
//
//      void updateMovement(float dt, ::graphics3d::scene_object* pobject);
//      void updateLook(float xOffset, float yOffset, ::graphics3d::scene_object * pobject);
//
//      void processMouseMovement(float xOffset, float yOffset);
//      void processKeyboardInput(float deltaTime);
//      void handleMouseInput();
//
//      glm::vec3 getCameraDirection() const;
//      glm::vec3 getCameraPosition() const;
//
//
//      ::pointer<::cube::impact > m_pimpact;
//      ::pointer<::graphics3d::key_map > m_pkeymap;
//
//      auto key(auto ekey)
//      {
//
//         return m_pkeymap->key(ekey);
//
//      }
//
//      //public:
//          //double lastX = 400.0;
//          //double lastY = 300.0;
//          //bool firstMouse = true;
//
//   public:
//
// 
//
//      // Existing functions...
//      //void handleMouseInput(GLFWwindow* window);
//
//      float getX() const { return m_Δx; }
//      float getY() const { return m_Δy; }
//
//
//   };
//
//
//}  // namespace graphics3d_vulkan
//
//

