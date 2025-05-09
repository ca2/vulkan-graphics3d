#include "framework.h"
#include "vk_input.h"
#include "vk_gameObject.h"
#include "lowland/landen/vulkan/vk_container.h"
#include <limits>

namespace vkc {

   MNKController::MNKController(float sensitivity, float yaw, float pitch)
      : _sensitivity(sensitivity), _yaw(yaw), _pitch(pitch), _cameraDirection(glm::vec3(0.0f, 0.0f, -1.0f)), _cameraPosition(glm::vec3(0.0f, 0.0f, 3.0f)) {
   }
   //void MNKController::moveInPlaneXZ(
   //    vkc::VkContainer * pvkcontainer, float dt, VkcGameObject& gameObject) {
   //    glm::vec3 rotate{ 0 };
   //    if (pvkcontainer->get_key_state(m_keymap.lookRight) == ::user::e_key_state_pressed) rotate.y += 1.f;
   //    if (pvkcontainer->get_key_state(m_keymap.lookLeft) == ::user::e_key_state_pressed) rotate.y -= 1.f;
   //    if (pvkcontainer->get_key_state(m_keymap.lookUp) == ::user::e_key_state_pressed) rotate.x += 1.f;
   //    if (pvkcontainer->get_key_state(m_keymap.lookDown) == ::user::e_key_state_pressed) rotate.x -= 1.f;

   //    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
   //        gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
   //    }

   //    // limit pitch values between about +/- 85ish degrees
   //    gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
   //    gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

   //    float yaw = gameObject.transform.rotation.y;
   //    const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
   //    const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
   //    const glm::vec3 upDir{ 0.f, -1.f, 0.f };

   //    glm::vec3 moveDir{ 0.f };
   //    if (pvkcontainer->get_key_state(m_keymap.moveForward) == ::user::e_key_state_pressed) moveDir += forwardDir;
   //    if (pvkcontainer->get_key_state(m_keymap.moveBackward) == ::user::e_key_state_pressed) moveDir -= forwardDir;
   //    if (pvkcontainer->get_key_state(m_keymap.moveRight) == ::user::e_key_state_pressed) moveDir += rightDir;
   //    if (pvkcontainer->get_key_state(m_keymap.moveLeft) == ::user::e_key_state_pressed) moveDir -= rightDir;
   //    if (pvkcontainer->get_key_state(m_keymap.moveUp) == ::user::e_key_state_pressed) moveDir += upDir;
   //    if (pvkcontainer->get_key_state(m_keymap.moveDown) == ::user::e_key_state_pressed) moveDir -= upDir;

   //    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
   //        gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
   //    }

   //    if (pvkcontainer->get_key_state(m_keymap.Exit) == ::user::e_key_state_pressed)
   //    {
   //       //glfwSetWindowShouldClose(window, GLFW_TRUE);

   //    }
   //}



   void MNKController::processMouseMovement(float xOffset, float yOffset)
   {
      // Apply sensitivity factor
      xOffset *= _sensitivity;
      yOffset *= _sensitivity;

      // Update yaw and pitch based on offsets
      _yaw += xOffset;
      _pitch += yOffset;

      // Constrain the pitch if necessary
      if (_pitch > 89.0f)
         _pitch = 89.0f;
      if (_pitch < -89.0f)
         _pitch = -89.0f;

      // Update camera direction based on yaw and pitch
      glm::vec3 direction;
      direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
      direction.y = sin(glm::radians(_pitch));
      direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
      _cameraDirection = glm::normalize(direction);
   }
   //void MNKController::processKeyboardInput(GLFWwindow* window, float deltaTime) {
   //    float cameraSpeed = 2.5f * deltaTime; // adjust speed as necessary

   //    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
   //        _cameraPosition += _cameraDirection * cameraSpeed;
   //    }
   //    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
   //        _cameraPosition -= _cameraDirection * cameraSpeed;
   //    }
   //    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
   //        _cameraPosition -= glm::normalize(glm::cross(_cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
   //    }
   //    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
   //        _cameraPosition += glm::normalize(glm::cross(_cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
   //    }
   //}


   void MNKController::updateLook(vkc::VkContainer* pvkcontainer, float xOffset, float yOffset, VkcGameObject& gameObject) {
      xOffset *= _sensitivity;
      yOffset *= _sensitivity;

      // limit pitch values between about +/- 85ish degrees
      gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
      gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
      if (pvkcontainer->is_absolute_mouse_position())
      {
         _yaw = xOffset;
         _pitch = yOffset;
      }
      else
      {

         _yaw += xOffset;
         _pitch += yOffset;

      }

      // Clamp pitch to avoid flipping
      _pitch = glm::clamp(_pitch, -89.0f, 89.0f);

      // Optional: wrap yaw
      if (_yaw > 360.0f) _yaw -= 360.0f;
      if (_yaw < 0.0f) _yaw += 360.0f;

      gameObject.transform.rotation.x = glm::radians(_pitch);
      gameObject.transform.rotation.y = glm::radians(_yaw);
   }


   void MNKController::updateMovement(vkc::VkContainer* pvkcontainer, float dt, VkcGameObject& gameObject)
   {
      float yaw = gameObject.transform.rotation.y;
      const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
      const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
      const glm::vec3 upDir{ 0.f, -1.f, 0.f };

      glm::vec3 moveDir{ 0.f };
      if (pvkcontainer->get_key_state(m_keymap.moveForward) == ::user::e_key_state_pressed) moveDir += forwardDir;
      if (pvkcontainer->get_key_state(m_keymap.moveBackward) == ::user::e_key_state_pressed) moveDir -= forwardDir;
      if (pvkcontainer->get_key_state(m_keymap.moveRight) == ::user::e_key_state_pressed) moveDir += rightDir;
      if (pvkcontainer->get_key_state(m_keymap.moveLeft) == ::user::e_key_state_pressed) moveDir -= rightDir;
      if (pvkcontainer->get_key_state(m_keymap.moveUp) == ::user::e_key_state_pressed) moveDir += upDir;
      if (pvkcontainer->get_key_state(m_keymap.moveDown) == ::user::e_key_state_pressed) moveDir -= upDir;

      if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
         gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
      }

      if (pvkcontainer->get_key_state(m_keymap.Exit) == ::user::e_key_state_pressed)
      {
         pvkcontainer->m_bShouldClose = true;

      }

   }


   glm::vec3 MNKController::getCameraDirection() const { return _cameraDirection; }

   glm::vec3 MNKController::getCameraPosition() const { return _cameraPosition; }

   void MNKController::handleMouseInput(vkc::VkContainer* pvkcontainer)
   {
      double xpos, ypos;
      double newx, newy;
      if (pvkcontainer->is_absolute_mouse_position())
      {
         newx = pvkcontainer->m_dMouseLastX * 1.25 * MATH_PI;
         newy = pvkcontainer->m_dMouseLastY * 1.25 * MATH_PI/2.0;
      }
      else
      {

         newx = pvkcontainer->m_dMouseLastX;
         newy = pvkcontainer->m_dMouseLastY;

      }
      //glfwGetCursorPos(window, &xpos, &ypos);

      //if (pvkcontainer->m_bFirstMouse) {
      //   _lastX = newx;
      //   _lastY = newy;
      //   pvkcontainer->m_bFirstMouse = false;
      //   xpos = _lastX;
      //   ypos = _lastY;
      //}
      //else
      if (!pvkcontainer->is_absolute_mouse_position())
      {

         if (pvkcontainer->m_bFirstMouse)
         {
            _lastX = newx;
            _lastY = newy;
            pvkcontainer->m_bFirstMouse = false;

         }

      }
      {



      }

      if (pvkcontainer->is_absolute_mouse_position())
      {

         xpos = _lastX + (newx - _lastX) * 0.05;
         ypos = _lastY + (newy - _lastY) * 0.05;
         _x = xpos;
         _y = -ypos;  // reversed Y
      }
      else
      {

         xpos = newx;
         ypos = newy;

         _x = _x + static_cast<float>(xpos - _lastX - _x) * 0.05;
         _y = _y + static_cast<float>(_lastY - ypos - _y) * 0.05;  // reversed Y


      }

      _lastX = xpos;
      _lastY = ypos;
   }
} // namespace vkc