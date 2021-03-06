// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
// 
// Modified from Apache Arrow's CSV reader by Kira Noël.
// 
// Copyright © Her Majesty the Queen in Right of Canada, as represented
// by the Minister of Statistics Canada, 2019.
//
// Distributed under terms of the license.

#ifndef FWFR_CONVERTER_H
#define FWFR_CONVERTER_H

#include <fwfr/options.h>
#include <fwfr/parser.h>

#include <cstring>
#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include <arrow/builder.h>
#include <arrow/memory_pool.h>
#include <arrow/status.h>
#include <arrow/type.h>
#include <arrow/type_traits.h>
#include <arrow/util/macros.h>
#include <arrow/util/parsing.h>  // IWYU pragma: keep
#include <arrow/util/trie.h>
#include <arrow/util/visibility.h>

namespace arrow {
    class Array;
    class DataType;
    class MemoryPool;
    class Status;
}

namespace fwfr {

class BlockParser;

class ARROW_EXPORT Converter {
 public:
  Converter(const std::shared_ptr<arrow::DataType>& type, const ConvertOptions& options,
            arrow::MemoryPool* pool);
  virtual ~Converter() = default;

  virtual arrow::Status Convert(const BlockParser& parser, int32_t col_index,
                                std::shared_ptr<arrow::Array>* out) = 0;

  std::shared_ptr<arrow::DataType> type() const { return type_; }

  static arrow::Status Make(const std::shared_ptr<arrow::DataType>& type, 
                            const ConvertOptions& options,
                            std::shared_ptr<Converter>* out);
  static arrow::Status Make(const std::shared_ptr<arrow::DataType>& type,
                            const ConvertOptions& options,
                            arrow::MemoryPool* pool, std::shared_ptr<Converter>* out);

 protected:
  ARROW_DISALLOW_COPY_AND_ASSIGN(Converter);

  virtual arrow::Status Initialize() = 0;

  const ConvertOptions options_;
  arrow::MemoryPool* pool_;
  std::shared_ptr<arrow::DataType> type_;
};

}  // namespace fwfr

#endif  // FWFR_CONVERTER_H
