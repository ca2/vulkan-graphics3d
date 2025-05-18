#pragma once 
// MNK_Controller.h
#include "application_object.h"
//#include "window.h"
#include "acme/constant/user_key.h"
#include <glm/glm.hpp>

namespace graphics3d_vulkan 
{


   class CLASS_DECL_GRAPHICS3D_VULKAN MNKController
   {
   public:

      MNKController(float sensitivity = 0.1f, float yaw = -90.0f, float pitch = 0.0f);

      struct KeyMap {
         ::user::e_key moveLeft = ::user::e_key_a;
         ::user::e_key moveRight = ::user::e_key_d;
         ::user::e_key moveForward = ::user::e_key_w;
         ::user::e_key moveBackward = ::user::e_key_s;
         ::user::e_key moveUp = ::user::e_key_e;
         ::user::e_key moveDown = ::user::e_key_q;
         ::user::e_key lookLeft = ::user::e_key_left;
         ::user::e_key lookRight = ::user::e_key_right;
         ::user::e_key lookUp = ::user::e_key_up;
         ::user::e_key lookDown = ::user::e_key_down;
         ::user::e_key Exit = ::user::e_key_escape;
      } m_keymap;

      //void moveInPlaneXZ(::cube::container * pcontainer, float dt, application_object& gameObject);

      void updateMovement(::cube::container* pcontainer, float dt, ::cube::application_object* papplicationobject);
      void updateLook(::cube::container* pcontainer, float xOffset, float yOffset, ::cube::application_object * papplicationobject);

      void processMouseMovement(float xOffset, float yOffset);
      void processKeyboardInput(::cube::container* pcontainer, float deltaTime);
      void handleMouseInput(::cube::container* pcontainer);

      glm::vec3 getCameraDirection() const;
      glm::vec3 getCameraPosition() const;

      //KeyMappings keys{};
      float moveSpeed{ 3.f };
      float lookSpeed{ 1.5f };
      bool   m_bMouseAbsolute;

      //public:
          //double lastX = 400.0;
          //double lastY = 300.0;
          //bool firstMouse = true;

   public:
      // Existing functions...
      //void handleMouseInput(GLFWwindow* window);

      float getX() const { return m_Δx; }
      float getY() const { return m_Δy; }

   private:
      double m_dMouseLastX = 0.0;
      double m_dMouseLastY = 0.0;
      double m_Δx = 0.;
      double m_Δy = 0.;




      float _sensitivity;  // Sensitivity factor for mouse movement
      float _yaw;          // Camera yaw (horizontal rotation)
      float _pitch;        // Camera pitch (vertical rotation)
      glm::vec3 _cameraDirection; // Camera direction vector
      glm::vec3 _cameraPosition;
   };


}  // namespace graphics3d_vulkan



