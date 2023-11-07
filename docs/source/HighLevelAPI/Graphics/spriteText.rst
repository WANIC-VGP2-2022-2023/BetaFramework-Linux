Sprite Text
===========

.. cpp:namespace:: Beta

.. cpp:class:: SpriteText : public Sprite

Methods
-------

.. cpp:function:: SpriteText::SpriteText(const std::string& text = "", FontPtr font = nullptr, bool wrap = false, unsigned rowLength = 10)
   
   Create a new sprite text object.
   
   :param text: The text to display.
   
   :param font: The font to use for this text. If null, will attempt to use a bitmap font via the sprite's SpriteSource.
   
   :param wrap: Whether to wrap text onto multiple lines.
   
   :param rowLength: Characters to print before wrapping.

.. cpp:function:: void SpriteText::Initialize() override
   
   Get component dependencies.

.. cpp:function:: void SpriteText::Update(float dt) override
   
   Update function.
   
   :param dt: The change in time (in seconds) since the last game loop.

.. cpp:function:: void SpriteText::Draw() override
   
   Draw the sprite text.

.. cpp:function:: void SpriteText::SetText(const std::string& text)
   
   Sets the text that will be displayed by the sprite.
   
   :param text: The text that will be displayed.

.. cpp:function:: const std::string& SpriteText::GetText() const
   
   Gets the text being displayed by the sprite.

.. cpp:function:: void SpriteText::SetFont(FontPtr font)
   
   Sets the font used to draw this text.
   
   :param font: A pointer to the font that should be used to render the text. Passing 0 to this function will cause the text to be drawn using the sprite's SpriteSource.

.. cpp:function:: void SpriteText::Serialize(FileStream& stream) const
   
   Save the SpriteText data to file.
   
   :param stream: The stream object used to save the SpriteText's data.

.. cpp:function:: void SpriteText::Deserialize(FileStream& stream)
   
   Load the SpriteText data from file.
   
   :param stream: The stream object used to load the SpriteText's data.
   
.. cpp:function:: static FontManager& SpriteText::GetFontManager()

	Returns the font manager, which can be used to load new fontes
	or obtain existing font resources.
	
	.. note:: To simplify the process of finding font resources, use the *ResourceGetFont* macro. This macro expands to *SpriteText::GetFontManager().GetResource(f)*, where *f* is the name of the font resource. When specifying the name of the font resource, be sure to include both the name and the size. Example: *ResourceGetFont("Arial.ttf 20")*
	
	:return: A reference to the font manager.
