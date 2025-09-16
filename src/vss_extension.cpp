#include "vss_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension/extension_loader.hpp"
#include "duckdb/parser/parsed_data/create_scalar_function_info.hpp"

#include "hnsw/hnsw.hpp"

namespace duckdb {

static void LoadInternal(ExtensionLoader &loader) {
	// Register the HNSW index module
	HNSWModule::Register(loader);
}

void VssExtension::Load(ExtensionLoader &loader) {
	LoadInternal(loader);
}

std::string VssExtension::Name() {
	return "vss";
}

} // namespace duckdb

extern "C" {

DUCKDB_CPP_EXTENSION_ENTRY(vss, loader) {
	duckdb::LoadInternal(loader);
}
}
