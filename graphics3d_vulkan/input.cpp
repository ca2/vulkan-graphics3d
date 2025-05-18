#include "framework.h"
#include "input.h"
#include "scene_object.h"
#include "app-cube/cube/impact.h"
#include "app-cube/cube/graphics3d/types.h"
#include <limits>


namespace graphics3d_vulkan
{


   MNKController::MNKController(float sensitivity, float yaw, float pitch)
   {
      _sensitivity = sensitivity;
      _yaw = yaw;
      _pitch = pitch;
      _cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
      _cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
   }
   //void MNKController::moveInPlaneXZ(
   //    ::cube::impact * pimpact, float dt, application_object& gameObject) {
   //    glm::vec3 rotate{ 0 };
   //    if (key(e_key_lookRight) == ::user::e_key_state_pressed) rotate.y += 1.f;
   //    if (key(e_key_lookLeft) == ::user::e_key_state_pressed) rotate.y -= 1.f;
   //    if (key(e_key_lookUp) == ::user::e_key_state_pressed) rotate.x += 1.f;
   //    if (key(e_key_lookDown) == ::user::e_key_state_pressed) rotate.x -= 1.f;

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
   //    if (key(e_key_moveForward) == ::user::e_key_state_pressed) moveDir += forwardDir;
   //    if (key(e_key_moveBackward) == ::user::e_key_state_pressed) moveDir -= forwardDir;
   //    if (key(e_key_moveRight) == ::user::e_key_state_pressed) moveDir += rightDir;
   //    if (key(e_key_moveLeft) == ::user::e_key_state_pressed) moveDir -= rightDir;
   //    if (key(e_key_moveUp) == ::user::e_key_state_pressed) moveDir += upDir;
   //    if (key(e_key_moveDown) == ::user::e_key_state_pressed) moveDir -= upDir;

   //    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
   //        gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
   //    }

   //    if (key(e_key_Exit) == ::user::e_key_state_pressed)
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


   void MNKController::updateLook(float xOffset, float yOffset, ::graphics3d::scene_object * pobject)
   {

      xOffset *= _sensitivity;
      yOffset *= _sensitivity;

      // limit pitch values between about +/- 85ish degrees
      pobject->m_transform.rotation.x = glm::clamp(pobject->m_transform.rotation.x, -1.5f, 1.5f);
      pobject->m_transform.rotation.y = glm::mod(pobject->m_transform.rotation.y, glm::two_pi<float>());

      if (m_pimpact->is_absolute_mouse_position())
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

      pobject->m_transform.rotation.x = glm::radians(_pitch);
      pobject->m_transform.rotation.y = glm::radians(_yaw);

   }


   void MNKController::updateMovement(float dt, ::graphics3d::scene_object* pobject)
   {

      float yaw = pobject->m_transform.rotation.y;
      const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
      const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
      const glm::vec3 upDir{ 0.f, -1.f, 0.f };

      glm::vec3 moveDir{ 0.f };
      {
         using namespace ::graphics3d;
         if (key(e_key_moveForward) == ::user::e_key_state_pressed) moveDir += forwardDir;
         if (key(e_key_moveBackward) == ::user::e_key_state_pressed) moveDir -= forwardDir;
         if (key(e_key_moveRight) == ::user::e_key_state_pressed) moveDir += rightDir;
         if (key(e_key_moveLeft) == ::user::e_key_state_pressed) moveDir -= rightDir;
         if (key(e_key_moveUp) == ::user::e_key_state_pressed) moveDir += upDir;
         if (key(e_key_moveDown) == ::user::e_key_state_pressed) moveDir -= upDir;

         if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
            pobject->m_transform.translation += moveSpeed * dt * glm::normalize(moveDir);
         }

         if (key(e_key_Exit) == ::user::e_key_state_pressed)
         {
            m_pimpact->m_bShouldClose = true;

         }

      }

   }


   glm::vec3 MNKController::getCameraDirection() const { return _cameraDirection; }

   glm::vec3 MNKController::getCameraPosition() const { return _cameraPosition; }

   void MNKController::handleMouseInput()
   {
      
      double x, y;
      double newx, newy;

      if (m_pimpact->is_absolute_mouse_position())
      {

         newx = m_pimpact->m_dCursorX * 1.25 * MATH_PI;
         newy = m_pimpact->m_dCursorY * 1.25 * MATH_PI/2.0;

      }
      else
      {

         newx = m_pimpact->m_dCursorX;
         newy = m_pimpact->m_dCursorY;

      }
      //glfwGetCursorPos(window, &xpos, &ypos);

      //if (m_pimpact->m_bFirstMouse) {
      //   _lastX = newx;
      //   _lastY = newy;
      //   m_pimpact->m_bFirstMouse = false;
      //   xpos = _lastX;
      //   ypos = _lastY;
      //}
      //else
      if (!m_pimpact->is_absolute_mouse_position())
      {

         if (m_pimpact->m_bFirstMouse)
         {
            m_dMouseLastX = newx;
            m_dMouseLastY = newy;
            m_pimpact->m_bFirstMouse = false;

         }

      }

      if (m_pimpact->is_absolute_mouse_position())
      {

         x = m_dMouseLastX + (newx - m_dMouseLastX) * 0.05;
         y = m_dMouseLastY + (newy - m_dMouseLastY) * 0.05;
         m_Δx = x;
         m_Δy = -y;  // reversed Y

      }
      else
      {

         x = newx;
         y = newy;

         m_Δx = m_Δx + static_cast<float>(x - m_dMouseLastX - m_Δx) * 0.05;
         m_Δy = m_Δy + static_cast<float>(m_dMouseLastY - y - m_Δy) * 0.05;  // reversed Y


      }

      m_dMouseLastX = x;
      m_dMouseLastY = y;

   }


} // namespace graphics3d_vulkan


