#include <graphics/Image.h>
//-------------------------------------------------------------------------------------------------
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <filesystem>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

Image::Image()
	: m_data(nullptr)
	, m_width(0)
	, m_height(0)
	, m_channels(0)
{
}

//-------------------------------------------------------------------------------------------------

Image::~Image()
{
	if (m_data != nullptr)
		stbi_image_free(m_data);
}

//-------------------------------------------------------------------------------------------------

void Image::load(std::string filename)
{
	std::filesystem::path imagepath(filename);
	if (!std::filesystem::exists(imagepath))
		return;

	m_data = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, 0);
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------

