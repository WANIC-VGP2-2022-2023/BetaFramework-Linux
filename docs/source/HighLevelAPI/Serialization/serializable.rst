Serializable
============

.. cpp:namespace:: Beta

.. cpp:class:: Serializable

Methods
-------

.. cpp:function:: virtual void Serializable::Serialize(FileStream& stream) const
   
   Saves object data to a file.
   
   :param stream: The stream for the file we want to write to.

.. cpp:function:: virtual void Serializable::Deserialize(FileStream& stream)
   
   Loads object data from a file.
   
   :param stream: The stream for the file we want to read from.