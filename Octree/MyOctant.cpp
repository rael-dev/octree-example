/*----------------------------------------------------
Programmer: Israel Anthony (israelanthonyjr@gmail.com)
Date: 08/2023
----------------------------------------------------*/

#include "MyOctant.h"
using namespace Simplex;

// Static variables
uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 0;
uint MyOctant::m_uIdealEntityCount = 0;

MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();

	m_uOctantCount = 0;
	m_uID = m_uOctantCount;
	m_pRoot = this;
	m_uOctantCount++;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uMaxLevel = a_nMaxLevel;
	m_uLevel = 0;

	// Create a vector collection that holds the global min/max of every entity in the scene
	std::vector<vector3> entityMinMaxList; 

	// Loop through every entity in the EntityMngr
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{	
		// Access the RigidBody's attached to the Entities to get the global min/max values
		entityMinMaxList.push_back(m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMinGlobal()); 
		entityMinMaxList.push_back(m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetMaxGlobal());
	}

	// Create a RigidBody using all of the points stored in the entityMinMaxList collection
	MyRigidBody* rbForAll = new MyRigidBody(entityMinMaxList); 

	// Use the center and the halfwidth to get the Min and Max of the Octant
	m_v3Center = rbForAll->GetCenterLocal();
	vector3 rbHalfWidth = rbForAll->GetHalfWidth();
	float hwMax = std::max({ rbHalfWidth.x, rbHalfWidth.y, rbHalfWidth.z });
	m_fSize = hwMax * 2.0f;
	m_v3Min = m_v3Center - vector3(hwMax);
	m_v3Max = m_v3Center + vector3(hwMax);

	ConstructTree(m_uMaxLevel); // Make the tree from the passed level
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();

	m_uOctantCount++;

	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	// Use the center and the size to get the Min and Max of the octant
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);
}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	m_pRoot, other.m_pRoot;
	m_pParent = other.m_pParent;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_fSize = other.m_fSize;

	m_lChild, other.m_lChild;
	m_uNumChildren = other.m_uNumChildren;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
}

MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	if (this == &other)
	{
		return *this;
	}
	else 
	{
		// Call Init to make this octant default again
		Release();
		Init();

		// Use the swap method to swap this octant with the 'other' octant
		MyOctant toSwap(other);
		Swap(toSwap); 
	}
}

Simplex::MyOctant::~MyOctant(void)
{
	Release(); // Deletes the octant in memory
}

void Simplex::MyOctant::Swap(MyOctant & other)
{
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_pParent, other.m_pParent);
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_fSize, other.m_fSize);

	std::swap(m_lChild, other.m_lChild);
	std::swap(m_pChild, other.m_pChild);
	std::swap(m_uNumChildren, other.m_uNumChildren);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	for (uint i = 0; i < 8; i++) 
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return m_v3Max;
}

vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return m_v3Min;
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	uint numEntities = m_pEntityMngr->GetEntityCount();

	// Stop checking if the index is out of bounds
	if (a_uRBIndex < 0 || a_uRBIndex >= numEntities) { return false; }

	// Get the min/max values from the rigidbody of the entity at the given index
	MyRigidBody* otherRB = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();
	vector3 otherMin = otherRB->GetMinGlobal();
	vector3 otherMax = otherRB->GetMaxGlobal();

	// Check for AABB collisions
	if (m_v3Max.x < otherMin.x) { return false; }
	if (m_v3Min.x > otherMax.x) { return false; }

	if (m_v3Max.y < otherMin.y) { return false; }
	if (m_v3Min.y > otherMax.y) { return false; }

	if (m_v3Max.z < otherMin.z) { return false; }
	if (m_v3Min.z > otherMax.z) { return false; }

	return true;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// Check if the member ID of this octant matches the index that was passed in
	if (m_uID == a_nIndex) 
	{
		// Add the wireframe of the octant to the MeshMngr
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)),
			a_v3Color,
			RENDER_WIRE);
	}
	else
	{
		// Check if the index is to display the children of the octant
		for (uint i = 0; i < m_uNumChildren; i++)
		{
			m_pChild[i]->Display(a_nIndex);
		}
	}
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	// Display the Octree
	for (uint i = 0; i < m_uNumChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color);
	}

	// Add the wireframe of the octant to the MeshMngr
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)),
		a_v3Color,
		RENDER_WIRE);
}

void Simplex::MyOctant::ClearEntityList(void)
{
	// Clear entity lists of the Octree
	for (uint i = 0; i < m_uNumChildren; i++)
	{
		m_pChild[i]->ClearEntityList();
	}
	m_EntityList.clear();
}

void Simplex::MyOctant::Subdivide(void)
{
	if (m_uNumChildren != 0) { return; } // If there are already children, do not subdivide again

	if (m_uLevel == m_uMaxLevel) { return; } // Cap at the MaxLevel 

	vector3 offset = ZERO_V3;
	float step = m_fSize / 4.0f; // Distance of the offset to new octants is a quarter of the size of the original octant

	// Creates the 8 new octants for the subdivision
	for (uint i = 0; i < 8; i++) 
	{
		// Generates the centers for the new octants
		offset.x = ((i & 1) ? step : -step);
		offset.y = ((i & 2) ? step : -step);
		offset.z = ((i & 4) ? step : -step);
		m_pChild[i] = new MyOctant(m_v3Center + offset, (m_fSize / 2.0f));

		// Adjust the variables for the child octants
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;

		// Assign the ID of the child to the entities within it
		m_pChild[i]->AssignIDtoEntity();

		// Subdivide children if they contain more than the ideal entity count
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
		{
			m_pChild[i]->Subdivide();
		}
	}

	m_uNumChildren = 8;
}

MyOctant * Simplex::MyOctant::GetChild(uint a_nChild)
{
	if (a_nChild < 0 || a_nChild > 7) { return nullptr; } // Check if the index passed in is valid

	return m_pChild[a_nChild];
}

MyOctant * Simplex::MyOctant::GetParent(void)
{
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	if (m_uNumChildren == 0) { return true; }
	return false;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	// Compare the size of the entity list to the passed in parameter
	if (m_EntityList.size() > a_nEntities) { return true; }
	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
	if (m_uNumChildren == 0) // Check if there are branches to break
	{
		return;
	}

	for (uint i = 0; i < m_uNumChildren; i++)
	{
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
	}
	m_uNumChildren = 0;
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	if (m_uLevel != 0) { return; } 

	// Clear the Octree's variables 
	ClearEntityList();
	KillBranches();
	m_lChild.clear();

	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;

	// Assign ID to the entities
	AssignIDtoEntity();

	// Subdivide if contains more than ideal count
	if (ContainsMoreThan(m_uIdealEntityCount))
	{
		Subdivide();
	}

	// Make the list of entities
	ConstructList();
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	if (IsLeaf())
	{
		// Check all entities to see if they are colliding with the Octant's RigidBody. If so, assign them the Octant's ID
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			if (IsColliding(i)) // If colliding add it to this octant's entity list
			{
				m_EntityList.push_back(i);
				m_pEntityMngr->ClearDimensionSet(i); 
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
	else // If not a leaf keep going down the tree
	{
		for (uint i = 0; i < m_uNumChildren; i++)
		{
			m_pChild[i]->AssignIDtoEntity();
		}
	}
}

uint Simplex::MyOctant::GetOctantCount(void)
{
	return m_uOctantCount;
}

void Simplex::MyOctant::Release(void)
{
	if (m_uLevel == 0) { KillBranches(); } // Destroy all branches of root

	// Clear entity lists and children
	m_EntityList.clear();
	m_lChild.clear();
	m_uNumChildren = 0;
}

void Simplex::MyOctant::Init(void)
{
	// Initialize the Octant's variables
	m_pRoot = nullptr;
	m_pParent = nullptr;

	m_uLevel = 0;
	m_uID = m_uOctantCount;

	m_fSize = 0.0f;
	m_v3Center = ZERO_V3;
	m_v3Min = ZERO_V3;
	m_v3Max = ZERO_V3;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}

void Simplex::MyOctant::ConstructList(void)
{
	// Build list of what entities are within the octant
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}

	// Construct list for children
	for (uint i = 0; i < m_uNumChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}

}
