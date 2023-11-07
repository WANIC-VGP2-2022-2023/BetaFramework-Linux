.. Beta Framework documentation master file, created by
   sphinx-quickstart on Mon Jul  1 16:30:51 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Beta - A Game Development Framework
===================================

Welcome to the website for Beta, a C++ library for the development of simple 2D games and game engines. This framework is used primarily by students in one of DigiPen's high intensity year-long high school game programming courses.

What is Beta?
-------------

Beta is a lightweight framework for teaching the development of 2D games with the following specifications:

* Games developed with Beta run on modern Windows PCs
* Code is written using the C++ progamming language

Beta offers two distinct versions of the library, depending on your goals and needs:

* High-Level API: A *working engine* with an event system, game object architecture, audio, simple colliders, physics, sprites, limited object serialization & deserialization
* Low-Level API: A *barebones framework* that handles graphics via OpenGL, keyboard/mouse input, and miscellaneous Windows application nonsense

What Beta is not:

* A tool for beginning programmers
* An engine for making 3D games
* Ready-made for collaboration between programmers and artists and sound designers
* A full-fledged multi-platform engine with a working level editor and toolchain such as Unity, Unreal, GameMaker

Why use Beta?
-------------

Beta was created as a tool for teaching advanced high school programming students with prior game development experience. The goal of Beta:

1. Teach students how to learn a game engine API. This is done using the High-Level API.
2. Teach students how parts of a game engine are programmed. Once students are familiar with the High-Level API, they begin working with the Low-Level API and learn how to program the classes and interfaces they took for granted.
3. Once the previous steps have been completed, students should have a much better understanding of game engines. Even if they never have to program one in the future, this will still help them program games using an engine built by someone else.

Who do I blame for this?
------------------------

Beta was developed by Jeremy Kings, the instructor of the WANIC DigiPen Advanced Video Game Programming course. Jeremy obtained his Masters in Computer Science from DigiPen Institute of Technology in 2013 and has since spent over 4 years inflicting his hard-earned knowledge on students and teachers alike.

While the overall scope of Beta is unlikely to change, you are free to send bug reports and minor suggestions to the following email: <j.kings@digipen.edu>

Where do I start?
-----------------

.. warning:: As of this time, the libraries and source code for Beta are only available to WANIC students via their (private) course websites.

Follow the links on the left sidebar to get started working with Beta:

* :doc:`Getting Started <GettingStarted/index>` shows you everything you need to install or download before you can start programming.
* :doc:`Tutorials <Tutorials/index>` will walk you through making some simple games using the High-Level API.
* :doc:`High-Level API <HighLevelAPI/index>` and :doc:`Low-Level API <LowLevelAPI/index>` provide detailed documentation and examples for classes and functions in the Beta framework.


.. toctree::
	:hidden:
	:includehidden:
	:maxdepth: 1
	:titlesonly:
	
	GettingStarted/index
	Tutorials/index
	HighLevelAPI/index
	LowLevelAPI/index