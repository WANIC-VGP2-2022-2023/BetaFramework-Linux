//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteText.h"

#include <sstream> // stringstream

// Components
#include "Area.h"

// Systems
#include "GameObject.h" // GetComponent
#include "FileStream.h"
#include <GraphicsEngine.h> // SetTransform

// Resources
#include <Font.h>

//------------------------------------------------------------------------------

namespace Beta
{
	FontManager SpriteText::fontManager;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	FontManager::FontManager()
		: ResourceManager("Fonts/", ".ttf")
	{
	}

	FontPtr FontManager::Create(const std::string& faceAndSizeString)
	{
		// Extract size from name string
		size_t sizeLocation = faceAndSizeString.find_last_of(' ');
		unsigned size = atoi(faceAndSizeString.substr(sizeLocation).c_str());

		// Return the desired font
		return FontPtr(Font::CreateFontFromFile(
			faceAndSizeString.substr(0, sizeLocation), size));
	}

	// Create a new sprite text object.
	// Params:
	//   text = The text to display.
	//   font = The font to use for this text. If null, will attempt
	//      to use a bitmap font via the sprite's SpriteSource.
	//   wrap = Whether to wrap text onto multiple lines
	//   rowLength = Characters to print before wrapping.
	SpriteText::SpriteText(const std::string& text, FontPtr font,
		bool wrap, unsigned rowLength)
		: text(text), wrap(wrap), rowLength(rowLength), numRows(0),
		maxColumns(0), area(nullptr), font(font)
	{
		SetName("SpriteText");
	}

	// Get component dependencies.
	void SpriteText::Initialize()
	{
		Sprite::Initialize();
		area = GetOwner()->GetComponent<Area>();
		SetText(text);
	}

	// Update function
	void SpriteText::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Update area component properties
		area->SetSize(Vector2D(static_cast<float>(maxColumns),
			static_cast<float>(numRows)));
		area->SetZDepth(GetZDepth());
	}

	void SpriteText::Draw()
	{
		if (font != nullptr)
		{
			DrawTextFont();
		}
		else
		{
			DrawTextBitmap();
		}
	}

	void SpriteText::SetText(const std::string& text_)
	{
		text = text_;

		// Calculate size and offset
		size_t length = text.size();

		unsigned column = 0;
		maxColumns = 0;
		numRows = 1;
		for (size_t i = 0; i < length; ++i)
		{
			// New lines and spaces
			if (text[i] == '\n' || (text[i] == ' ' && column >= rowLength))
			{
				++numRows;
				column = 0;
			}
			else
			{
				++column;
				maxColumns = std::max(column, maxColumns);
			}
		}
	}

	const std::string& SpriteText::GetText() const
	{
		return text;
	}

	void SpriteText::SetFont(FontPtr _font)
	{
		font = _font;
	}

	void SpriteText::Serialize(FileStream& stream) const
	{
		Sprite::Serialize(stream);
		stream.WriteVariable("text", text);
	}

	void SpriteText::Deserialize(FileStream& stream)
	{
		Sprite::Deserialize(stream);
		stream.ReadVariable("text", text);
	}

	FontManager& SpriteText::GetFontManager()
	{
		return fontManager;
	}

	// Draws text using a bitmap (sprite source).
	void SpriteText::DrawTextBitmap()
	{
		// Set left of text based on text size or row length
		size_t length = text.size();
		Vector2D offset;
		if (wrap)
		{
			offset.x -= transform->GetScale().x * (static_cast<float>(maxColumns + 1) / 2.0f);
			offset.y += (transform->GetScale().y / 2.0f) * (numRows - 1);
		}
		else
		{
			offset.x -= transform->GetScale().x * (static_cast<float>(length + 1) / 2.0f);
		}

		unsigned column = 0;
		float baseOffsetX = offset.x;
		for (size_t i = 0; i < length; ++i)
		{
			// New lines and spaces
			if (text[i] == '\n' || (text[i] == ' ' && column >= rowLength))
			{
				offset.y -= transform->GetScale().y;
				offset.x = baseOffsetX;
				column = 0;
			}
			else
			{
				++column;
				offset.x += transform->GetScale().x;
			}

			unsigned frame = text[i] - 32;
			SetFrame(frame);

			// Draw at the specified offset
			Sprite::Draw(offset);
		}
	}

	// Draws text using a font file.
	void SpriteText::DrawTextFont()
	{
		// Set left of text based on text size or row length
		size_t length = text.size();
		Vector2D offset;

		// Wrapping behavior
		if (wrap)
		{
			// Adjust initial offset to maintain parity with bitmap fonts, area component
			// TO DO: Find a way to do this that doesn't involve numbers that are quite so magical.
			offset = Vector2D(maxColumns * 0.35f, -0.28f);

			offset.x -= transform->GetScale().x * (static_cast<float>(maxColumns + 1) / 2.0f);
			offset.y += (transform->GetScale().y / 2.0f) * (numRows - 1);
		}
		else
		{
			// Adjust initial offset to maintain parity with bitmap fonts, area component
			// TO DO: Find a way to do this that doesn't involve numbers that are quite so magical.
			offset = Vector2D(length * 0.35f, -0.28f);

			offset.x -= transform->GetScale().x * (static_cast<float>(length + 1) / 2.0f);
		}

		unsigned column = 0;
		std::stringstream textToPrint;
		GraphicsEngine & graphics = *EngineGetModule(GraphicsEngine);
		graphics.SetSpriteBlendColor(GetColor());

		for (size_t i = 0; i < length; ++i)
		{
			// New lines and spaces
			if (text[i] == '\n' || (text[i] == ' ' && column >= rowLength))
			{
				// Set transform
				graphics.SetTransform(offset + transform->GetTranslation(),
					transform->GetScale(), 0.0f, GetZDepth());

				// Draw at the specified offset
				font->DrawText(textToPrint.str());

				// Clear the contents of the stream
				textToPrint.str(std::string());
				textToPrint.clear();

				// Reset offsets
				offset.y -= transform->GetScale().y;
				column = 0;
			}
			else
			{
				++column;
				textToPrint << text[i];
			}
		}

		// Print any remaining text
		std::string lastText = textToPrint.str();
		if (!lastText.empty())
		{
			// Set transform
			graphics.SetTransform(offset + transform->GetTranslation(),
				transform->GetScale(), 0.0f, GetZDepth());

			// Draw at the specified offset
			font->DrawText(lastText);
		}
	}

	// RTTI
	COMPONENT_SUBCLASS_SUBCLASS_DEFINITION(Sprite, SpriteText)
}

//------------------------------------------------------------------------------
