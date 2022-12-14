/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi_d3d12_texture.h"


gapi_d3d12_texture2d::gapi_d3d12_texture2d(t::shared_ptr<d3d12_texture2d> d3dtetxure)
    : super()
    , m_d3d12_texture(d3dtetxure)
{
}
