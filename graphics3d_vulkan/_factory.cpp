// Created by camilo on 2025-05-17 02:54 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "engine.h"
#include "model.h"
#include "app-cube/cube/application_object.h"
//#include "networking.h"
//#include "apex/networking/http/message.h"
//#include "sockets/basic/socket_handler.h"
//#include "sockets/basic/tcp_socket.h"
//#include "sockets/basic/listen_socket_impl.h"


IMPLEMENT_FACTORY(graphics3d_vulkan)
{

   pfactory->add_factory_item < ::graphics3d_vulkan::model, ::cube::model >();

   pfactory->add_factory_item < ::graphics3d_vulkan::engine, ::cube::engine >();

   pfactory->add_factory_item < ::graphics3d_vulkan::context, ::cube::context >();

   pfactory->add_factory_item < ::cube::application_object >();

}



