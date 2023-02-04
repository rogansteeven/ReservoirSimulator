#include "Simulator\Data.hpp"
#include "Simulator\Model.hpp"
#include "Simulator\Simulator.hpp"
#include "Simulator\Typedef.hpp"
#include "Simulator\Block.hpp"
#include "Simulator\Test.hpp"

#include <iostream>

int main()
{
	DataSourceMap dataSources = {
		{DataSource::P2, "Data\\project2.txt"},
		{DataSource::P4, "Data\\well_data.txt"}
	};

	auto data = Data::Create(dataSources);
	auto model = Model::Create(data);
	Simulator::Init(data, model);

	Test::Data1(data);
}