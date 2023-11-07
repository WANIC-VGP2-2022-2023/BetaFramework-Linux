High-Level API Reference
========================

.. cpp:namespace:: Beta

Overview
--------

This page contains links to documentation for each of the classes in the High-Level API. Note that the 
High-Level API does depend on the Low-Level API, so at some point you may need to reference the Low-Level 
documentation as well.

For example, most system modules contained in the High-Level API are added to the application and accessed 
using the :cpp:class:`EngineCore` class.

Classes
-------

Core

* :doc:`Area <Core/area>`
* :doc:`Component <Core/component>`
* :doc:`GameObject <Core/gameObject>`
* :doc:`GameObjectFactory <Core/gameObjectFactory>`
* :doc:`GameObjectManager <Core/gameObjectManager>`
* :doc:`Transform <Core/transform>`

Audio

* :doc:`SoundEvent <Audio/soundEvent>`
* :doc:`SoundManager <Audio/soundManager>`

Events

* :doc:`EventManager <Events/eventManager>`
* :doc:`Reactive <Events/reactive>`

Graphics

* :doc:`Animation <Graphics/animation>`
* :doc:`Animator <Graphics/animator>`
* :doc:`Sprite <Graphics/sprite>`
* :doc:`SpriteSource <Graphics/spriteSource>`
* :doc:`SpriteText <Graphics/spriteText>`
* :doc:`SpriteTilemap <Graphics/spriteTilemap>`

Levels

* :doc:`Level <Levels/level>`
* :doc:`MapObjectSpawner <Levels/mapObjectSpawner>`
* :doc:`Space <Levels/space>`
* :doc:`SpaceManager <Levels/spaceManager>`
* :doc:`Tilemap <Levels/tilemap>`

Math

* :doc:`Intersection2D <Math/intersection2D>`

Physics

* :doc:`Collider <Physics/collider>`
* :doc:`ColliderCircle <Physics/colliderCircle>`
* :doc:`ColliderLine <Physics/colliderLine>`
* :doc:`ColliderRectangle <Physics/colliderRectangle>`
* :doc:`ColliderTilemap <Physics/colliderTilemap>`
* :doc:`Quadtree <Physics/quadtree>`
* :doc:`RigidBody <Physics/rigidbody>`

Serialization

* :doc:`Serializable <Serialization/serializable>`

.. toctree::
   :hidden:
   :maxdepth: 2
   :titlesonly:
   
   Core/index
   Audio/index
   Events/index
   Graphics/index
   Levels/index
   Math/index
   Physics/index
   Serialization/index