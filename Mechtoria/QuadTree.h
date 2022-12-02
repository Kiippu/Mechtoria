#pragma once
#include <memory>
#include "Vec2.h"

struct Voxel;

struct Quad
{
    const Vec2<float> topLeft;
    const Vec2<float> bottomRight;
    Vec2<float> getMidPoint() const
    {
        float x = { topLeft.GetX() + ((bottomRight.GetX() - topLeft.GetX()) / 2)};
        float y = { topLeft.GetY() + ((bottomRight.GetY() - topLeft.GetY()) / 2)};
        return Vec2<float>{ x, y };
    }
};


template < class dataType>
struct QuadNode
{
    const QuadNode<dataType>*               parent;
    std::unique_ptr<QuadNode<dataType>>     children[4];
    const Quad                              area;
    //const uint8_t                           depth;
    //bool                                    active;
    dataType*                               data;
    QuadNode(QuadNode<dataType>* _parent, Quad _quad/*, uint8_t _depth, bool _active = false */ )
        : area(_quad), /*depth(_depth), active(_active),*/ data(nullptr), parent(_parent)
    {}
};

enum class TreeSize
{
    SQUARED_4 = 1,
    SQUARED_16 = 2,
    SQUARED_64 = 3,
    SQUARED_256 = 4,
    SQUARED_1024 = 5,
    SQUARED_4096 = 6,
    SQUARED_16384 = 7,
    SQUARED_65536 = 8,
    SQUARED_262144 = 9,
    UNLIMITED = INT_MAX
};

template <class dataType>
class QuadTree
{
public:
    QuadTree(TreeSize _treeSize, Vec2<float> _origin)
        : m_treeSize(_treeSize), m_position(_origin)
    {
        m_rootQuadNode = std::make_unique<QuadNode<dataType>>(nullptr, Quad{ Vec2<float>(0.f,0.f), Vec2<float>(1.f,1.f) });
        int depth{ 0 };
        PopulateTree(m_rootQuadNode.get(), depth);
        //DebugPrint();
    }

    QuadNode<dataType>* GetFirstNode();
    //void DebugPrint();

private:
    TreeSize m_treeSize;
    Vec2<float> m_position;
    std::unique_ptr<QuadNode<dataType>> m_rootQuadNode;

private:
    void PopulateTree(QuadNode<dataType>* _parent, int& _currentTreeDept);
};

//template<class dataType>
//inline void QuadTree<dataType>::DebugPrint()
//{
//    auto getSize = [&](QuadNode<dataType>* _parent, int& _count) {
//        _count++;
//        for (auto& node : _parent->children)
//            getSize(node.get(), _count);
//    };
//    int count = 0;
//    getSize(m_rootQuadNode.get(), count);
//    printf("total node count:" + count);
//}

template<class dataType>
inline QuadNode<dataType>* QuadTree<dataType>::GetFirstNode()
{
    return m_rootQuadNode.get();
}

template<class dataType>
inline void QuadTree<dataType>::PopulateTree(QuadNode<dataType>* _parent, int& _currentTreeDepth)
{
    // hit max tree depth to return here and decrement depth counter
    if (_currentTreeDepth >= (int)m_treeSize)
    {
        --_currentTreeDepth;
        return;
    }
    {
        int childDepth = _currentTreeDepth + 1;
        // top and dims
        const Vec2<float>& TOP_LEFT = _parent->area.topLeft;
        const Vec2<float>& BOT_RIGHT = _parent->area.bottomRight;
        Vec2<float> DIMS = BOT_RIGHT - TOP_LEFT;
        Vec2<float> HALF_DIMS = DIMS * .5f;
        Vec2<float> TOP_MID = TOP_LEFT + Vec2<float>{HALF_DIMS.GetX(), 0.f};
        Vec2<float> TOP_RIGHT = TOP_LEFT + Vec2<float>{DIMS.GetX(), 0.f};
        // middle 
        Vec2<float> MID_LEFT = TOP_LEFT + Vec2<float>{0.f, HALF_DIMS.GetY()};
        Vec2<float> MID_MID = TOP_LEFT + HALF_DIMS;
        Vec2<float> MID_RIGHT = BOT_RIGHT - Vec2<float>{0.f, HALF_DIMS.GetY()};
        // bottom
        Vec2<float> BOT_LEFT = TOP_LEFT + Vec2<float>{0.f, DIMS.GetY()};
        Vec2<float> BOT_MID = BOT_RIGHT - Vec2<float>{HALF_DIMS.GetX(), 0.f};
        // populate children 
        _parent->children[0] = std::make_unique<QuadNode<Voxel>>(_parent, Quad{ TOP_LEFT, MID_MID });
        _parent->children[1] = std::make_unique<QuadNode<Voxel>>(_parent, Quad{ TOP_MID, TOP_RIGHT });
        _parent->children[2] = std::make_unique<QuadNode<Voxel>>(_parent, Quad{ MID_LEFT, BOT_MID });
        _parent->children[3] = std::make_unique<QuadNode<Voxel>>(_parent, Quad{ MID_MID, BOT_RIGHT });
    }
    // recursively buildeach child
    for (auto& node : _parent->children)
        PopulateTree(node.get(), ++_currentTreeDepth);
}
