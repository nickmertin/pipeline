//
// Created by nmeritn on 2018-06-03.
//

#include "pipeline.h"
#include "source.h"
#include "sink.h"

namespace pipeline {
    template <>
    class source<int>;

    template <>
    class sink<int>;
}
