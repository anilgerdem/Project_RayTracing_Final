#pragma once

#include "swHittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

namespace sw {
    class Hittable_list : public Hittable {
  public:
    Hittable_list() = default;
    Hittable_list(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec) const;

    public:
    std::vector<shared_ptr<Hittable>> objects;
    };

    bool Hittable_list::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
        hit_record temp_record;
        bool hit_anything = false;
        float temp_closest = t_max;

        for (const auto &object : objects) {
            if (object->hit(r, t_min, temp_closest, temp_record)) {
                hit_anything = true;
                temp_closest = temp_record.t;
                rec = temp_record;
            }
        }
        return hit_anything;
    }


} // namespace sw
