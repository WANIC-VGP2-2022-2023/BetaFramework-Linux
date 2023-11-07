Level
=====

.. cpp:namespace:: Beta

.. cpp:class:: Level : public BetaObject

Methods
-------

.. cpp:function:: Level::Level(const std::string& name)
   
   Creates a game state.
   
   :param name: The name to use for this state.

.. cpp:function:: Space* Level::GetSpace() const
   
   Retrieve the space in which the level is running.
   
   :return: A pointer to the space in which this level is loaded.