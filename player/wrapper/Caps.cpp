#include "Caps.hpp"

Gst::Caps::Caps(GstCaps* handler) :
    m_handler(handler)
{
}

Gst::Caps::Caps(const std::string& stringCaps) :
    m_stringCaps(stringCaps),
    m_handler(gst_caps_from_string(m_stringCaps.c_str()))
{
}

Gst::Caps::Caps(Gst::Caps&& other)
{
    std::swap(m_handler, other.m_handler);
}

Gst::Caps& Gst::Caps::operator=(Gst::Caps&& other)
{
    std::swap(m_handler, other.m_handler);
    return *this;
}

Gst::Caps::~Caps()
{
    gst_caps_unref(m_handler);
}

Gst::RefPtr<Gst::Caps> Gst::Caps::create(const std::string& stringCaps)
{
    return std::make_shared<Gst::Caps>(stringCaps);
}

Gst::RefPtr<Gst::Structure> Gst::Caps::getStructure(guint index) const
{
    return std::make_shared<Gst::Structure>(gst_caps_get_structure(m_handler, index));
}

GstCaps* Gst::Caps::getHandler()
{
    return m_handler;
}

Gst::Structure::Structure(GstStructure* handler) :
    m_handler(handler)
{
}

int Gst::Structure::getHeight() const
{
    int height;
    gst_structure_get_int(m_handler, "height", &height);
    return height;
}

int Gst::Structure::getWidth() const
{
    int width;
    gst_structure_get_int(m_handler, "width", &width);
    return width;
}
