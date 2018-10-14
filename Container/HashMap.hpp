#ifndef _HASH_MAP_HPP_
#define _HASH_MAP_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Container/Container.hpp>
#include <Core/Hash.hpp>
#include <string.h>

/******************************************************************************
**	Defines
******************************************************************************/
#define HASH_MAP_STACK_CAPACITY		32
#define HASH_MAP_BLOCK_ALLOCATION	1

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	template <class KEY, class TYPE>
	class HashMap
	{
	public:
		HashMap();
		~HashMap();
	
		void					Add(const KEY& _kKey, const TYPE& _kValue);
		void					Remove(const KEY& _kKey);

		TYPE&					Get			(const KEY& _kKey, const TYPE& _kDefault) const;

		inline uint32			GetSize() const { return m_vNode.GetSize(); }
		inline uint32			GetCapacity() const { return m_vNode.GetCapacity(); }
		inline bool				IsEmpty() const { return m_vNode.IsEmpty(); }
		inline void				Clear() { memset(&m_vNode[0], -1, sizeof(Node) * m_vNode.GetCapacity()); m_vNode.Clear(); }

		void					operator=	(const HashMap<KEY, TYPE>& _mValue) { m_vNode = _mValue.m_vNode; };
		TYPE&					operator[]	(const KEY& _kKey) const;

	private:
		/******************************************************************************
		**	Node
		******************************************************************************/
		struct Node
		{
			struct EState
			{
				enum Type
				{
					Removed = -1,
					Unused = 0,
					Used = 1,
				};
			};

			Node() : State(EState::Unused) { }

			inline bool		IsAvailable() const { return State <= 0; }

			TYPE					Value;
			KEY						Key;
			typename EState::Type	State;
		};

	public:
		/******************************************************************************
		**	Iterator
		******************************************************************************/
		class Iterator
        {
			friend class HashMap;

        private:
            Iterator(Node* _pNode, Node* _pLast) : m_pNode(_pNode), m_pLast(_pLast) { }

		public:
			~Iterator() { }

			inline KEY GetKey() const { return m_pNode->Key; }
			inline TYPE& GetValue() { return m_pNode->Value; }

			inline TYPE& operator*() { return m_pNode->Value; }
            inline TYPE& operator->() { return m_pNode->Value; }
			inline bool operator==(const Iterator& _it) const { return m_pNode == _it.m_pNode; }
			inline bool operator!=(const Iterator& _it) const { return m_pNode != _it.m_pNode; }
			inline Iterator& operator++() { while(++m_pNode != m_pLast && m_pNode->State != Node::EState::Used); return *this; }
            inline Iterator operator++(int /*iValue*/) { Iterator kThis = *this; ++kThis; return kThis; }

        private:
            Node* m_pNode;
			Node* m_pLast;
        };

		inline Iterator			GetFirst();
		inline Iterator			GetLast();

	private:
		inline Node*			FindNode(const KEY& _kKey) { return const_cast<Node*>(FindNode(&m_vNode[0], m_vNode.GetCapacity(), _kKey)); }
		inline const Node*		FindNode(const KEY& _kKey) const { return FindNode(&m_vNode[0], m_vNode.GetCapacity(), _kKey); }
		const Node*				FindNode(const Node* _pArray, uint32 _uiCapacity, const KEY& _kKey) const;

		void					Reserve(uint32 _uiCapacity);

	private:
		Node			m_aStack[HASH_MAP_STACK_CAPACITY];
		Vector<Node>	m_vNode; 
	};

	/******************************************************************************
	**	Class Definition
	******************************************************************************/

	//!	@brief		ComputeIndex	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	uint32 ComputeIndex(const KEY& _kKey, uint32 _uiArraySize)
	{
		uint32 uiHash = Hash::Generate(_kKey);
		return uiHash & (_uiArraySize - 1);
	}

	//!	@brief		Constructor	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	HashMap<KEY, TYPE>::HashMap()
	{
		//Container::Set(m_aStack, -1, HASH_MAP_STACK_CAPACITY * sizeof(Node));
		m_vNode.SetBuffer(m_aStack, 0, HASH_MAP_STACK_CAPACITY, false);
	}

	//!	@brief		Destructor	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	HashMap<KEY, TYPE>::~HashMap()
	{
		// Nothing to do
	}

	//!	@brief		Add	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	void HashMap<KEY, TYPE>::Add(const KEY& _kKey, const TYPE& _kValue)
	{
		// If not present so we need to allocate more space
		Node* pNode = FindNode(_kKey);
		while(!pNode)
		{
			Reserve(m_vNode.GetCapacity() << HASH_MAP_BLOCK_ALLOCATION);
			pNode = FindNode(_kKey);
		}

		// Apply size only if needed
		if(pNode->IsAvailable())
		{
			m_vNode.Resize(m_vNode.GetSize()+1);
		}
		
		// Update key and value associated
		pNode->Key = _kKey;
		pNode->Value = _kValue;
		pNode->State = Node::EState::Used;
	}

	//!	@brief		Remove	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	void HashMap<KEY, TYPE>::Remove(const KEY& _kKey)
	{
		Node* pNode = FindNode(_kKey);
		if (pNode)
		{
			pNode->State = Node::EState::Removed;
			m_vNode.Resize(m_vNode.GetSize()-1);
		}
	}

	//!	@brief		Get	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	TYPE& HashMap<KEY, TYPE>::Get(const KEY& _kKey, const TYPE& _kDefault) const
	{
		const Node* pNode = FindNode(_kKey);
		if(pNode && pNode->State == Node::EState::Used)
		{
			return const_cast<TYPE&>(pNode->Value);
		}
		return const_cast<TYPE&>(_kDefault);
	}

	//!	@brief		GetFirst	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	typename HashMap<KEY, TYPE>::Iterator HashMap<KEY, TYPE>::GetFirst()
	{
		Iterator it(&m_vNode[0], &m_vNode[m_vNode.GetCapacity()]);
		if(it.m_pNode->State != Node::EState::Used)
		{
			++it;
		}

		return it;
	}

	//!	@brief		GetLast	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	typename HashMap<KEY, TYPE>::Iterator HashMap<KEY, TYPE>::GetLast()
	{
		Node* pLastNode = &m_vNode[m_vNode.GetCapacity()]; 
		return Iterator(pLastNode, pLastNode);
	}

	//!	@brief		FindNode	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	const typename HashMap<KEY, TYPE>::Node* HashMap<KEY, TYPE>::FindNode(const typename HashMap<KEY, TYPE>::Node* _pArray, uint32 _uiCapacity, const KEY& _kKey) const
	{
		uint32 iStep = 1;
		uint32 uiNodeIndex = ComputeIndex<KEY, TYPE>(_kKey, _uiCapacity);
		while (uiNodeIndex < _uiCapacity)
		{
			const HashMap<KEY, TYPE>::Node* pNode = &_pArray[uiNodeIndex];
			if (pNode->IsAvailable() || _kKey == pNode->Key)
			{
				return pNode;
			}
			uiNodeIndex += iStep++;
		}

		return nullptr;
	}

	//!	@brief		Reserve	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	void HashMap<KEY, TYPE>::Reserve(uint32 _uiCapacity)
	{
		const uint32 uiCapacity = m_vNode.GetCapacity();
		if (_uiCapacity > uiCapacity)
		{
			// Keep reference of old array
			Node* pElement = &m_vNode[0];
			Node* pEnd = &m_vNode[uiCapacity];

			// Allocate new array and set proper capacity
			Node* pNewArray = new Node[_uiCapacity];
			//memset(pNewArray, -1, _uiCapacity * sizeof(Node));

			// Iterates old array to reaffect all iterator
			while (pElement != pEnd)
			{
				if (pElement->State != Node::EState::Unused)
				{
					Node* pNode = const_cast<Node*>(FindNode(pNewArray, _uiCapacity, pElement->Key));
					*pNode = *pElement;
				}
				++pElement;
			}

			m_vNode.SetBuffer(pNewArray, m_vNode.GetSize(), _uiCapacity, true);
		}
	}

	//!	@brief		operator[]	
	//!	@date		2015-09-19
	template <typename KEY, typename TYPE>
	TYPE& HashMap<KEY, TYPE>::operator[](const KEY& _kKey) const
	{
		const Node* pNode = FindNode(_kKey);
		if(pNode && pNode->State == Node::EState::Used)
		{
			return const_cast<TYPE&>(pNode->Value);
		}
		
		static const TYPE DefaultValue = (TYPE)-1;
		return const_cast<TYPE&>(DefaultValue);
	}
}
#undef HASH_MAP_BLOCK_ALLOCATION

#endif