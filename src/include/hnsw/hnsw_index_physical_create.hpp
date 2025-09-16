#pragma once
#include "duckdb/execution/physical_operator.hpp"
#include "duckdb/execution/progress_data.hpp"
#include "duckdb/storage/data_table.hpp"

namespace duckdb {

class DuckTableEntry;

class PhysicalCreateHNSWIndex : public PhysicalOperator {
public:
	static constexpr const PhysicalOperatorType TYPE = PhysicalOperatorType::EXTENSION;

public:
	PhysicalCreateHNSWIndex(PhysicalPlan &physical_plan, const vector<LogicalType> &types_p, TableCatalogEntry &table,
	                        const vector<column_t> &column_ids, unique_ptr<CreateIndexInfo> info,
	                        vector<unique_ptr<Expression>> unbound_expressions, idx_t estimated_cardinality);

	//! The table to create the index for
	DuckTableEntry &table;
	//! The list of column IDs required for the index
	vector<column_t> storage_ids;
	//! Info for index creation
	unique_ptr<CreateIndexInfo> info;
	//! Unbound expressions to be used in the optimizer
	vector<unique_ptr<Expression>> unbound_expressions;
	//! Whether the pipeline sorts the data prior to index creation
	const bool sorted;

public:
	//! Source interface, NOOP for this operator
	SourceResultType GetData(ExecutionContext &context, DataChunk &chunk, OperatorSourceInput &input) const override {
		return SourceResultType::FINISHED;
	}
	bool IsSource() const override {
		return true;
	}

public:
	//! Sink interface, thread-local sink states
	unique_ptr<LocalSinkState> GetLocalSinkState(ExecutionContext &context) const override;
	//! Sink interface, global sink state
	unique_ptr<GlobalSinkState> GetGlobalSinkState(ClientContext &context) const override;
	SinkResultType Sink(ExecutionContext &context, DataChunk &chunk, OperatorSinkInput &input) const override;
	SinkCombineResultType Combine(ExecutionContext &context, OperatorSinkCombineInput &input) const override;
	SinkFinalizeType Finalize(Pipeline &pipeline, Event &event, ClientContext &context,
	                          OperatorSinkFinalizeInput &input) const override;

	bool IsSink() const override {
		return true;
	}
	bool ParallelSink() const override {
		return true;
	}

	ProgressData GetSinkProgress(ClientContext &context, GlobalSinkState &gstate,
	                             ProgressData source_progress) const override;
};

} // namespace duckdb
