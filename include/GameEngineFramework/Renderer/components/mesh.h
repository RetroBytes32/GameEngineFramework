#ifndef __COMPONENT_MESH
#define __COMPONENT_MESH

#include <vector>

#include "../../engine/types/bufferlayout.h"

#include "../../engine/types/color.h"
#include "../../Math/Math.h"

#include "../../configuration.h"

#ifndef RENDERER_VERTEX_BUFFER_MAX
  #define RENDERER_VERTEX_BUFFER_MAX  100000
#endif

struct ENGINE_API SubMesh {
    
    /// Begin offset position.
    unsigned vertexBegin;
    unsigned vertexCount;
    unsigned indexBegin;
    unsigned indexCount;
    
    /// Current mesh position offset.
    glm::vec3 position;
    
    /// Vertex buffer array.
    std::vector<Vertex>  vertexBuffer;
    /// Index buffer array.
    std::vector<Index>   indexBuffer;
    
};



class ENGINE_API Mesh {
    
public:
    
    friend class RenderSystem;
    
    Mesh();
    ~Mesh();
    
    /// Reallocate new GPU memory.
    void Reallocate(unsigned int newBufferSize);
    
    /// Set the primitive drawing type for the vertex data.
    void SetPrimitive(int primitiveType);
    
    /// Set and enable a vertex attribute layout.
    void SetAttribute(int index, int attributeCount, int vertexSize, int byteOffset);
    /// Disable a vertex attribute.
    void DisableAttribute(int index);
    
    /// Load vertex buffer data onto the GPU.
    void LoadVertexBuffer(Vertex* bufferData, int vertexCount);
    /// Load index buffer data onto the GPU.
    void LoadIndexBuffer(Index* bufferData, int indexCount);
    
    /// Bind the mesh buffer for rendering.
    void Bind(void);
    /// Run a draw call on this vertex buffer.
    void DrawVertexArray(void);
    /// Run a draw call on this index buffer.
    void DrawIndexArray(void);
    
    /// Return the number of sub meshes in this vertex buffer.
    unsigned int GetSubMeshCount(void);
    
    /// Add a plain sub mesh to the vertex buffer.
    void AddPlain(float x, float y, float z, float width, float height, Color color, float uCoord=1, float vCoord=1, float uStart=0, float vStart=0, unsigned int uOffset=0, unsigned int vOffset=0);
    /// Add a sub divided plain sub mesh to the vertex buffer.
    void AddPlainSubDivided(float x, float y, float z, float width, float height, Color color, unsigned int widthSub, unsigned int heightSub);
    
    /// Add a wall sub mesh to the vertex buffer.
    void AddWall(float x, float y, float z, float width, float height, Color color);
    /// Add a sub divided wall sub mesh to the vertex buffer.
    void AddWallSubDivided(float x, float y, float z, float width, float height, Color color, unsigned int widthSub, unsigned int heightSub);
    
    /// Add a sub mesh into this vertex buffer.
    bool AddSubMesh(float x, float y, float z, SubMesh& mesh, bool doUploadToGpu=true);
    /// Add a vertex buffer directly into this vertex buffer.
    bool AddSubMesh(float x, float y, float z, std::vector<Vertex>& vrtxBuffer, std::vector<Index>& indxBuffer, bool doUploadToGpu=true);
    
    /// Remove a sub mesh from this vertex buffer.
    bool RemoveSubMesh(unsigned int index);
    /// Copy this vertex buffer into a sub mesh.
    bool CopySubMesh(unsigned int index, SubMesh& mesh);
    /// Copy this vertex buffer directly into a vertex buffer.
    bool CopySubMesh(unsigned int index, std::vector<Vertex>& vrtxBuffer, std::vector<Index>& indxBuffer);
    
    /// Update the color of a sub mesh.
    bool ChangeSubMeshColor(unsigned int index, Color newColor);
    /// Update the position of a sub mesh.
    bool ChangeSubMeshPosition(unsigned int index, float x, float y, float z);
    
    /// Clear all sub meshes in the mesh.
    void ClearSubMeshes(void);
    
    /// Fully re-upload the vertex buffer into the GPU buffer.
    void UploadToGPU(void);
    
    /// Get the number of index locations in the index buffer.
    unsigned int GetNumberOfIndices(void);
    /// Get the number of vertex locations in the vertex buffer.
    unsigned int GetNumberOfVertices(void);
    
    /// Get a vertex from the internal vertex array.
    Vertex GetVertex(unsigned int index);
    /// Set a vertex to the internal vertex array.
    void SetVertex(unsigned int index, Vertex vertex);
    
    /// Get an index from the internal index array.
    Index GetIndex(unsigned int index);
    /// Set an index to the internal index array.
    void SetIndex(unsigned int index, Index position);
    
private:
    
    // OpenGL buffer indexes
    unsigned int mVertexArray;
    unsigned int mBufferVertex;
    unsigned int mBufferIndex;
    
    // Render draw type
    int mPrimitive;
    
    // Buffer sizes
    unsigned int mVertexBufferSz;
    unsigned int mIndexBufferSz;
    unsigned int mMaxSize;
    
    // Vertex buffer array
    std::vector<Vertex>   mVertexBuffer;
    // Index buffer array
    std::vector<Index>    mIndexBuffer;
    
    // List of sub meshes in this mesh
    std::vector<SubMesh> mSubMesh;
    // List of freed sub meshes in this mesh
    std::vector<SubMesh> mFreeMesh;
    
    // Apply default layout settings
    void SetDefaultAttributes(void);
    
    // Allocate/deallocate 
    void AllocateBuffers(unsigned int maxBufferSize);
    void FreeBuffers(void);
    
};



#endif
