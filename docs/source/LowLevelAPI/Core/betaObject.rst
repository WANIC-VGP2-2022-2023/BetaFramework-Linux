.. rst-class:: searchtitle

BetaObject
==========

.. rst-class:: searchdescription

BetaObject is an `abstract class <https://www.geeksforgeeks.org/pure-virtual-functions-and-abstract-classes/>`_ 
meant to serve as a common interface for objects that will be managed and updated by a game engine.

While you cannot make instances of this class directly, other classes can inherit from this class. 
Examples of High-Level API classes that inherit from this class include GameObject, Component, Level, 
Space, and EventManager, among others.

.. cpp:namespace:: Beta

.. cpp:class:: BetaObject

Methods
-------

	.. cpp:function:: BetaObject::BetaObject(const std::string& name, BetaObject* owner = nullptr)
	
		Construct a BetaObject.
		
		:param name: The name of the object.
		:param owner: The object that owns or contains this object.

	.. cpp:function:: BetaObject::BetaObject(const BetaObject& other)
	
		Construct a BetaObject by copying another object. The new object will share same name, 
		but will be given a different ID.
		
		:param other: The object whose properties will be used to construct this one.

	.. cpp:function:: virtual BetaObject::~BetaObject() = 0

		Destroy a BetaObject. This function is virtual to ensure destruction works properly with 
		inherited classes. It is PURE virtual (= 0) to force the class to be abstract without 
		forcing the user to implement any functions.

	.. cpp:function:: BetaObject& BetaObject::operator=(const BetaObject& other)
	
		Replace an object's properties with that of another object. The object will retain its ID, 
		but all other properties will be replaced.
		
		:param other: The object whose properties will replace this object's properties.
	
	.. cpp:function:: virtual void BetaObject::Load()

		Load external resources associated with this object. Typically called when a level starts for 
		the first time. Default implementation does nothing.
	
	.. cpp:function:: virtual void BetaObject::Initialize()

		Reset instance data and reinstantiates child objects. Typically called when a level is loaded 
		or restarted. Default implementation does nothing.
	
	.. cpp:function:: virtual void BetaObject::Update(float dt)
	
		Executes all operations that should occur every frame. Default implementation does nothing.
		
		:param dt: The change in time between last frame and the current frame.
	
	.. cpp:function:: virtual void BetaObject::FixedUpdate(float dt)
	
		Updates components using a fixed timestep (usually just for physics). 
		Default implementation does nothing.
		
		:param dt: A fixed change in time, usually 1/60th of a second.
	
	.. cpp:function:: virtual void BetaObject::Draw()
		
		Draw this object. Could be a sprite, text, or debug visuals. 
		Default implementation does nothing.
	
	.. cpp:function:: virtual void BetaObject::Shutdown()
	
		Removes any objects that were recreated in Initialize. Typically called 
		when restarting or changing levels. Default implementation does nothing.
		
	.. cpp:function:: virtual void BetaObject::Unload()
	
		Unloads external assets associated with this object. Typically called 
		when changing levels. Default implementation does nothing.
	
	.. cpp:function:: const std::string& BetaObject::GetName() const
	
		Return the name of the object.
		
	.. cpp:function:: void BetaObject::SetName(const std::string& name)
	
		Give the object a new name. Not usually recommended, as any code 
		depending on an object having a specific name is likely to break.
		
	.. cpp:function:: const IDType BetaObject::GetID() const
	
		Return the globally unique identifier of the object.
		
		:return: An unsigned integer representing the object's ID.
	
	.. cpp:function:: BetaObject* BetaObject::GetOwner() const
	
		Retrieve the object that owns or contains this object.
		
		:return: A pointer to a BetaObject, or *nullptr* if the object has no owner.
	
	.. cpp:function:: void BetaObject::SetOwner(BetaObject* object)
	
		Set the object that owns or contains this object.
		
		:param object: A pointer to the object that owns this object.
	
	.. cpp:function:: static BetaObject* BetaObject::GetObjectByID(IDType id)
	
		Return a pointer to an object with the given ID, if it exists.
		
		:param id: The ID of the object that should be retrieved.
		
		:return: A pointer to a BetaObject, or *nullptr* if the object does not exist (was destroyed).