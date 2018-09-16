#include <hitable.hpp>

namespace tracer {

hitable_list::hitable_list()
    : list{}
{

}

hitable_list::hitable_list(const std::vector<hitable *>&_list)
    : list{_list}
{
    
}

hitable_list::hitable_list(const hitable_list& hl)
    : list{hl.list}
{
    
}

hitable_list::hitable_list(hitable_list&& hl)
    : list{std::move(hl.list)}
{

}

hitable_list::~hitable_list()
{
    for (auto& it : list) {
        delete it;
    }

    list.clear();
}

bool hitable_list::hit(const ray &r, float t_min, 
                       float t_max, hit_record &record) const
{
    hit_record tmp_record;
    bool res = false;
    double closest = t_max;
    for (auto it : list) {
        if (it->hit(r, t_min, closest, tmp_record)) {
            res = true;
            closest = tmp_record.parameter;
            record = tmp_record;
        }
    }
    return res;
}

}