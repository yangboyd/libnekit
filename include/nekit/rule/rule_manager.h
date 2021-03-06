// MIT License

// Copyright (c) 2017 Zhuhao Wang

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <functional>
#include <memory>
#include <system_error>
#include <vector>

#include "../hedley/hedley.h"
#include <boost/asio.hpp>

#include "../utils/async_interface.h"
#include "../utils/cancelable.h"
#include "../utils/resolver_interface.h"
#include "../utils/result.h"
#include "rule_interface.h"

namespace nekit {
namespace rule {
enum class RuleManagerErrorCode { NoMatch = 1 };

class RuleManagerErrorCategory : public utils::ErrorCategory {
 public:
  NE_DEFINE_STATIC_ERROR_CATEGORY(RuleManagerErrorCategory)

  std::string Description(const utils::Error& error) const override;
  std::string DebugDescription(const utils::Error& error) const override;
};

class RuleManager final : public utils::AsyncInterface {
 public:
  using EventHandler =
      std::function<void(utils::Result<std::shared_ptr<RuleInterface>>&&)>;

  explicit RuleManager(utils::Runloop* runloop);

  ~RuleManager();

  void AppendRule(std::shared_ptr<RuleInterface> rule);

  HEDLEY_WARN_UNUSED_RESULT utils::Cancelable Match(
      std::shared_ptr<utils::Session> session, EventHandler handler);

  utils::Runloop* GetRunloop() override;

 private:
  void MatchIterator(
      std::vector<std::shared_ptr<RuleInterface>>::const_iterator iter,
      std::shared_ptr<utils::Session> session, utils::Cancelable cancelable,
      EventHandler handler);

  std::vector<std::shared_ptr<RuleInterface>> rules_;
  utils::Runloop* runloop_;
  utils::Cancelable lifetime_;
};

}  // namespace rule
}  // namespace nekit
