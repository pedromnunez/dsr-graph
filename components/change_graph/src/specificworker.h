/*
 *    Copyright (C) 2020 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
	\brief
	@author authorname
*/



#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include "../../../graph-related-classes/CRDT.h"
#include "../../../graph-related-classes/CRDT_graphviewer.h"
#include "../../../graph-related-classes/dsr_to_osg_viewer.h"


class SpecificWorker : public GenericWorker
{
Q_OBJECT

	public:
		std::shared_ptr<CRDT::CRDTGraph> G;
		std::string agent_name;

	private:
		int agent_id;
		std::string dsr_input_file;
		std::string dsr_output_file;
		std::string test_output_file;
		
		// graph viewer
		std::unique_ptr<DSR::GraphViewer> graph_viewer;

	public:
		SpecificWorker(TuplePrx tprx);
		~SpecificWorker();
		bool setParams(RoboCompCommonBehavior::ParameterList params);

	public slots:
		void compute();
		void initialize(int period);
		void change_node_slot(int id);
		void save_node_slot();
		void change_edge_slot(int id);
		void save_edge_slot();

	private:
		void fill_table(QTableWidget *table_widget, std::map<std::string, Attrib> attrib);
		std::map<std::string, Attrib> get_table_content(QTableWidget *table_widget, std::map<std::string, Attrib> attrs);


};
Q_DECLARE_METATYPE(Node);
Q_DECLARE_METATYPE(Edge);
#endif