//------------------------------------------------------------------------------
//
// File Name:	Font.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Font.h"

// OpenGL
#include <glad.h>
#include "../../glfw/src/glfw3.h"

// Windows
// No ^
//#include <Shlobj.h> // GetKnownFolderPath

// Font loading
#include <ft2build.h>
#include FT_FREETYPE_H

// Resources
#include <MeshFactory.h>

// Systems
#include "EngineCore.h"		// GetModule, GetFilePath
#include "GraphicsEngine.h" // GetFontSystem
#include "ShaderProgram.h"	// Use, SetUniform

//------------------------------------------------------------------------------

namespace Beta
{
		//------------------------------------------------------------------------------
		// Private Structures:
		//------------------------------------------------------------------------------

		std::string Font::fontPath = "Fonts/";
		static const int FT_Success = 0;
		static const int numCharacters = 128;
		typedef FT_FaceRec_ FontFace;

		// Many values in FreeType are stored as sub-pixels, i.e. 1 / 64 pixels
		// Bitshift by 6 to get value in pixels (2^6 = 64)
		inline unsigned SubPixelsToPixels(int value)
		{
			return value >> 6;
		}

		// Represents a character texture loaded from a font file
		struct Character
		{
			//------------------------------------------------------------------------------
			// Public Variables:
			//------------------------------------------------------------------------------

			Texture* texture;	 // Texture used for this character.
			Vector2D size;       // Size of glyph
			Vector2D bearing;    // Offset from baseline to left/top of glyph
			Vector2D advance;    // Offset to advance to next glyph
			FT_UInt glyphIndex;  // Index of the glyph for this character.

			//------------------------------------------------------------------------------
			// Public Functions:
			//------------------------------------------------------------------------------

			Character(Texture* texture, const Vector2D& size, const Vector2D& bearing,
				Vector2D advance, FT_UInt glyphIndex)
				: texture(texture), size(size), bearing(bearing), advance(advance), glyphIndex(glyphIndex)
			{
			}

			~Character()
			{
				delete texture;
			}

			// Disable copy constructor and assignment
			Character(const Character& other) = delete;
			Character& operator=(const Character& other) = delete;
		};
		typedef std::map<char, Character*> CharacterMap;

		class Font::Implementation
		{
		public:
			Implementation(const std::string& filename, unsigned size, FontFace* face)
				: filename(filename), size(size), characterMesh(nullptr), face(face),
				hasKerning(FT_HAS_KERNING(face)), graphics(*EngineGetModule(GraphicsEngine))
			{
				characterMesh = EngineGetModule(MeshFactory)->CreateQuadMesh(Vector2D(1, 1), Vector2D(0.5f, 0.5f));
			}

			~Implementation()
			{
				for (auto it = characters.begin(); it != characters.end(); ++it)
				{
					delete it->second;
				}

				delete characterMesh;

				// Unload font face now that we've gotten the data we need
				FT_Done_Face(face);
			}

			std::string filename;
			unsigned size;
			CharacterMap characters;
			Mesh* characterMesh;
			FontFace* face;
			bool hasKerning;
			GraphicsEngine& graphics;
		private:
		};

		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		Font::~Font()
		{
			delete pimpl;
		}

		// Draw text to the screen using this font.
		// (NOTE: Use SetTransform to define where the
		// text should appear on screen.)
		// Params:
		//   text = The string of text that should be drawn.
		void Font::DrawText(const std::string& text) const
		{
			UNREFERENCED_PARAMETER(text);

			// Activate corresponding render state
			GraphicsEngine& graphics = *EngineGetModule(GraphicsEngine);
			const ShaderProgram& fontShader = graphics.GetFontShader();
			fontShader.Use();

			// Positions of characters relative to each other
			Vector2D offset;

			// Record glyph for kerning
			FT_Vector kerning;
			kerning.x = 0; kerning.y = 0;
			unsigned previousGlyph = 0;
			float fontSizeFloat = static_cast<float>(pimpl->size);

			// Iterate through all characters
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character& ch = *pimpl->characters[*c];

				if (*c != '\n')
				{
					Vector2D meshSize = ch.size;

					// Get kerning information
					if (pimpl->hasKerning)
					{
						FT_UInt currentGlyph = ch.glyphIndex;
						if (previousGlyph != 0)
						{
							FT_Get_Kerning(pimpl->face, previousGlyph, currentGlyph,
								FT_KERNING_DEFAULT, &kerning);
							kerning.x = SubPixelsToPixels(kerning.x);
						}
						previousGlyph = currentGlyph;
					}

					// Incorporate kerning BEFORE making the mesh
					offset.x += static_cast<float>(kerning.x) / fontSizeFloat;

					Vector2D position = offset;
					position.x += ch.bearing.x;
					position.y -= (ch.size.y - ch.bearing.y);

					// Calculate vertices for character
					const size_t numVertices = 6;
					Vector2D positions[numVertices] = {
						position,
						{ position.x + meshSize.x, position.y },
						{ position.x, position.y + meshSize.y },

						{ position.x + meshSize.x, position.y },
						{ position.x + meshSize.x, position.y + meshSize.y },
						{ position.x, position.y + meshSize.y },
					};

					// Update VBO with new vertices
					pimpl->characterMesh->UpdatePositionBuffer(positions);

					// Use the appropriate texture
					ch.texture->Use(Vector2D(), &fontShader);

					// Draw the character using the given mesh
					pimpl->characterMesh->Draw();

					// Now advance cursors for next glyph
					offset.x += ch.advance.x;
				}
				else
				{
					// New line! Advance to next line using advance.y.
					offset.x = 0.0f;
					offset.y -= ch.advance.y;
					previousGlyph = 0;
				}
			}
		}

		const std::string& Font::GetName() const
		{
			return pimpl->filename;
		}

		unsigned Font::GetSize() const
		{
			return pimpl->size;
		}

		Font* Font::CreateFontFromFile(const std::string& filename, unsigned size)
		{
			FontFace* face = nullptr;

			GraphicsEngine& graphics = *EngineGetModule(GraphicsEngine);
			FontSystem* system = graphics.GetFontSystem();

            //TODO: FIND FONTS

			// Get system font path
			//PWSTR cWStringPath = nullptr;
			//PSTR systemFontPath = nullptr;
			//SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &cWStringPath);
			// Convert from wstring to string
			/*int pathSize = WideCharToMultiByte(CP_ACP, 0, cWStringPath, -1, nullptr, 0, nullptr, nullptr);
			int totalSize = static_cast<int>(pathSize + filename.size() + 1);
            freetype_
			systemFontPath = new char[totalSize];
			WideCharToMultiByte(CP_ACP, 0, cWStringPath, -1, systemFontPath, totalSize, nullptr, nullptr);
			CoTaskMemFree(cWStringPath);
			// Add slash and null terminator
			systemFontPath[pathSize - 1] = '\\';
			systemFontPath[totalSize - 1] = '\0';
			// Append filename
			memcpy(systemFontPath + pathSize, filename.c_str(), filename.size() * sizeof(char));

			// Attempt to load font file from system directory first
			if (FT_New_Face(system, systemFontPath, 0, &face) != FT_Success)
			{
				// Failing that, try engine font directory
				std::string filePath = EngineCore::GetInstance().GetFilePath() + fontPath + filename;
				if (FT_New_Face(system, filePath.c_str(), 0, &face) != FT_Success)
				{
					std::cout << "ERROR: Failed to load font " << filename << std::endl;
					return nullptr;
				}
			}
			delete[] systemFontPath;

			// Set size
			if (FT_Set_Pixel_Sizes(face, 0, size) != FT_Success)
			{
				std::cout << "ERROR: Failed to set font size for font " << filename << std::endl;
				return nullptr;
			}

			Font* font = new Font(filename, size, face);
			CharacterMap& characters = font->pimpl->characters;

			// Load basic ASCII set

			// Disable byte-alignment restriction; characters don't line up nicely.
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			for (GLubyte c = 0; c < numCharacters; c++)
			{
				// Load character glyph
				FT_UInt glyphIndex = FT_Get_Char_Index(face, c);
				if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER) != FT_Success)
				{
					std::cout << "ERROR: Failed to load glyph " << c << " in font " << filename << std::endl;
					continue;
				}
				// Generate texture
				Texture* texture = new Texture(face->glyph->bitmap.buffer,
					face->glyph->bitmap.width, face->glyph->bitmap.rows, filename
					+ " " + static_cast<char>(c));

				// Pre-calculate some values
				float sizeFloat = static_cast<float>(size);
				unsigned advanceX = SubPixelsToPixels(face->glyph->advance.x);
				unsigned advanceY = face->glyph->advance.y == 0 ? size
					: SubPixelsToPixels(face->glyph->advance.y);

				// Now store character for later use
				Character* character = new Character(texture,
					Vector2D(static_cast<float>(face->glyph->bitmap.width),
						static_cast<float>(face->glyph->bitmap.rows)) / sizeFloat,
					Vector2D(static_cast<float>(face->glyph->bitmap_left),
						static_cast<float>(face->glyph->bitmap_top)) / sizeFloat,
					Vector2D(static_cast<float>(advanceX), static_cast<float>(advanceY)) / sizeFloat,
					glyphIndex
				);

				characters.emplace(c, character);
			}

			// Set alignment back to normal (4 bytes)
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			// Return the (hopefully) complete font
			return font;
		}

		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		Font::Font(const std::string& filename, unsigned size, FontFace* face)
			: pimpl(new Implementation(filename, size, face))
		{*/
		}
}

//------------------------------------------------------------------------------
