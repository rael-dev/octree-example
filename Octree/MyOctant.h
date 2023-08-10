/*----------------------------------------------------
Programmer: Israel Anthony (israelanthonyjr@gmail.com)
Date: 08/2023
----------------------------------------------------*/

#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{
	class MyOctant
	{
		static uint m_uOctantCount; 
		static uint m_uMaxLevel;
		static uint m_uIdealEntityCount; 

		uint m_uID = 0; 
		uint m_uLevel = 0; 
		uint m_uNumChildren = 0; 

		float m_fSize = 0.0f;

		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr; 

		vector3 m_v3Center = vector3(0.0f); 
		vector3 m_v3Min = vector3(0.0f); 
		vector3 m_v3Max = vector3(0.0f); 

		MyOctant* m_pParent = nullptr;
		MyOctant* m_pChild[8];

		std::vector<uint> m_EntityList; 

		MyOctant* m_pRoot = nullptr;
		std::vector<MyOctant*> m_lChild;  

	public:
		/*
		USAGE: Constructor
		ARGUMENTS:
		- uint a_nMaxLevel = 2 -> Sets the maximum level of subdivision
		- uint nIdealEntityCount = 5 -> Sets the ideal level of objects per octant
		OUTPUT: class object
		*/
		MyOctant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);

		/*
		USAGE: Constructor
		ARGUMENTS:
		- vector3 a_v3Center -> Center of the octant in global space
		- float a_fSize -> size of each side of the octant volume
		OUTPUT: class object
		*/
		MyOctant(vector3 a_v3Center, float a_fSize);

		MyOctant(MyOctant const& other);
		MyOctant& operator=(MyOctant const& other);
		~MyOctant(void);

		/*
		USAGE: Swap the variables and children of the octants
		ARGUMENTS:
		- MyOctant& other -> object to swap content from
		*/
		void Swap(MyOctant& other);

		/*
		USAGE: Gets this octant's size
		OUTPUT: size of octant
		*/
		float GetSize(void);

		/*
		USAGE: Gets the center of the octant in global space
		OUTPUT: Center of the octant in global space
		*/
		vector3 GetCenterGlobal(void);

		/*
		USAGE: Gets the min corner of the octant in global space
		OUTPUT: Minimum in global space
		*/
		vector3 GetMinGlobal(void);

		/*
		USAGE: Gets the max corner of the octant in global space
		OUTPUT: Maximum in global space
		*/
		vector3 GetMaxGlobal(void);

		/*
		USAGE: Checks if there is a collision with the Entity specified by index 
		ARGUMENTS:
		- int a_uRBIndex -> Index of the Entity in the Entity Manager
		*/
		bool IsColliding(uint a_uRBIndex);

		/*
		USAGE: Displays the MyOctant volume specified by index including the objects underneath
		ARGUMENTS:
		- uint a_nIndex -> MyOctant to be displayed
		- vector3 a_v3Color = REYELLOW -> Color of the volume to display
		*/
		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);

		/*
		USAGE: Displays the MyOctant volume outlined in the specified color
		ARGUMENTS:
		- vector3 a_v3Color = C_YELLOW -> Color of the outline to display
		*/
		void Display(vector3 a_v3Color = C_YELLOW);

		/*
		USAGE: Clears the Entity list for each node
		*/
		void ClearEntityList(void);

		/*
		USAGE: Allocates 8 smaller child octants from this octant
		*/
		void Subdivide(void);

		/*
		USAGE: Returns the child specified by the index
		ARGUMENTS: uint a_nChild -> index of the child (from 0 to 7)
		OUTPUT: The child octant
		*/
		MyOctant* GetChild(uint a_nChild);

		/*
		USAGE: Returns the parent of the octant
		OUTPUT: The parent octant
		*/
		MyOctant* GetParent(void);

		/*
		USAGE: Checks if the octant does not contain any children 
		*/
		bool IsLeaf(void);

		/*
		USAGE: Checks to see if the octant contains more entities than the provided number
		ARGUMENTS:
		- uint a_nEntities -> Number of Entities to query
		*/
		bool ContainsMoreThan(uint a_nEntities);

		/*
		USAGE: Deletes all children and the children of their children (almost sounds apocalyptic)
		*/
		void KillBranches(void);

		/*
		USAGE: Creates a tree using subdivisions, the max number of objects and levels
		ARGUMENTS:
		- uint a_nMaxLevel = 3 -> Sets the maximum level of the tree while constructing it
		*/
		void ConstructTree(uint a_nMaxLevel = 3);

		/*
		USAGE: Traverse the tree up to the leafs and sets the objects in them to the index
		*/
		void AssignIDtoEntity(void);

		/*
		USAGE: Gets the total number of octants in the world
		*/
		uint GetOctantCount(void);

	private:
		/*
		USAGE: Deallocates member fields
		*/
		void Release(void);

		/*
		USAGE: Allocates member fields
		*/
		void Init(void);

		/*
		USAGE: creates the list of all leafs that contains objects.
		*/
		void ConstructList(void);
	};
} 

#endif //__OCTANTCLASS_H_

 