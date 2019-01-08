#include <bvh.hpp>
#include <utils.hpp>

#include <glm/gtx/string_cast.hpp>

namespace tracer
{

bool box_x_cmp(hitable* a, hitable* b)
{
    aabb box_left, box_right;
    if (!a->bounding_box(0,0,box_left) || !b->bounding_box(0,0,box_right))
        std::cerr << "no bounding box in bvh_node constructor (x)" << std::endl;
    if (box_left.min.x - box_right.min.x < 0.0f)
        return false;
    return true;
}

bool box_y_cmp(hitable* a, hitable* b)
{
    aabb box_left, box_right;
    if (!a->bounding_box(0,0,box_left) || !b->bounding_box(0,0,box_right))
        std::cerr << "no bounding box in bvh_node constructor (x)" << std::endl;
    if (box_left.min.y - box_right.min.y < 0.0f)
        return false;
    return true;
}

bool box_z_cmp(hitable* a, hitable* b)
{
    aabb box_left, box_right;
    if (!a->bounding_box(0,0,box_left) || !b->bounding_box(0,0,box_right))
        std::cerr << "no bounding box in bvh_node constructor (z)" << std::endl;
    if (box_left.min.z - box_right.min.z < 0.0f)
        return false;
    return true;
}

bvh_node::bvh_node()
{

}

bvh_node::bvh_node(std::vector<hitable*> list, float t0, float t1)
{
    int axis = int(3 * get_uniform_random());
    // std::cout << "axis " << axis << std::endl;
    switch(axis) {
    case 0:
        // std::cout << "case 0" << std::endl;
        std::sort(list.begin(), list.end(), box_x_cmp);
        break;
    case 1:
        // std::cout << "case 1" << std::endl;
        // std::sort(list.begin(), list.end(), box_x_cmp);
        // break;
    case 2:
    default:
        // std::cout << "case 2" << std::endl;
        std::sort(list.begin(), list.end(), box_z_cmp);
        break;
    }

    is_leaf = false;

    if (list.size() == 1) {
        left = right = list[0];
        is_leaf = true;
    }
    else if (list.size() == 2) {
        left = list[0];
        right = list[1];
        is_leaf = true;
    }
    else {
        const size_t half = list.size() / 2;
        std::vector<hitable*> left_list{list.begin(), list.begin() + half};
        std::vector<hitable*> right_list{list.begin() + half, list.end()};
        left = new bvh_node (left_list, t0, t1);
        right = new bvh_node(right_list, t0, t1);
    }

    aabb box_left, box_right;

    if (!left->bounding_box(t0, t1, box_left) || !right->bounding_box(t0, t1, box_right))
        std::cerr << "no bounding box in bvh_node constructor" << std::endl;
    
    bbox = aabb::surrounding_box(box_left, box_right);
}

void bvh_node::destroy(bvh_node* node) 
{
    if (!node->is_leaf) {
        bvh_node* l = (bvh_node*) node->left;
        bvh_node* r = (bvh_node*) node->right;
        destroy(l);
        destroy(r);
    }
    delete node;
    node = nullptr;
}

void bvh_node::destroy()
{
    if (!is_leaf) {
        bvh_node* l = (bvh_node*) left;
        bvh_node* r = (bvh_node*) right;
        destroy(l);
        destroy(r);
    }
}

bvh_node::~bvh_node()
{
}

bool bvh_node::hit(const ray& r, float t_min, float t_max, hit_record& record) const
{
    if (bbox.hit(r, t_min, t_max)) {
        hit_record left_rec, right_rec;
        bool hit_left = left->hit(r, t_min, t_max, left_rec);
        bool hit_right = right->hit(r, t_min, t_max, right_rec);

        if (hit_left && hit_right) {
            if (left_rec.parameter < right_rec.parameter)
                record = left_rec;
            else
                record = right_rec;
            return true;
        }
        else if (hit_left) {
            record = left_rec;
            return true;
        }
        else if (hit_right) {
            record = right_rec;
            return true;
        }
    }

    return false;
}

bool bvh_node::bounding_box(float t0, float t1, aabb& box) const
{
    box = bbox;
    return true;
}

}