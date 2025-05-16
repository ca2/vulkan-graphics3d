// Created by camilo on 2024-07-26 <3ThomasBorregaardSorensen!!
#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

/** @brief State of mouse/touch input */
struct mouse_state 
{
   struct {
      bool left = false;
      bool right = false;
      bool middle = false;
   } m_buttons;
   glm::vec2 position;
} ;
