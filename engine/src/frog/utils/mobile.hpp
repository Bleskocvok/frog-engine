#pragma once

namespace frog::mobile {


namespace ios {

bool is_ipad();

const char* save_path(const char* org, const char* app);

const char* asset_path();

} // namespace ios


namespace android {

const char* save_path();

} // namespace android


} // namespace frog
