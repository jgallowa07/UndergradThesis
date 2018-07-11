
//
// TREE SEQUENCE RECORDING
//
#pragma mark -
#pragma mark Tree sequence recording
#pragma mark -

// other tree sequence methods should probably be implemented here, unless you just make them inline forwards to code in treerec...

void
SLiMSim::handle_error(std::string msg, int err)
{
	node_table_print_state(&nodes,stdout);
	edge_table_print_state(&edges,stdout);
	std::cout << "Error:" << msg << ":" << msp_strerror(err) << std::endl;
    	exit(1);
}


void SLiMSim::simplifyTables(void){


//	std::cout << "--------- simplifyTables() -----------" << std::endl;

	//get all samples for simplification
	std::map<slim_objectid_t,Subpopulation*>::iterator it;
	std::vector<Individual*> subpopulationIndividuals;
	std::vector<Individual*> populationIndividuals;
	//std::vector<slim_pedigreeid_t> pedigreeIdOfAliveIndividuals;

	std::vector<node_id_t> samples;
	std::map<int,node_id_t> newMap;

	/*
	//In WF, This is all nodes in the tables with the lowest time. 
	if(ModelType() == SLiMModelType::kModelTypeWF){
	 
		std::cout << "IN WF CASE" << std::endl;	

		double minTime = 0.0;
		for (int i = 0; i < nodes.num_rows; i++){
			if(nodes.time[i] < minTime){
				minTime = nodes.time[i];
			}
		}
		for (int i = 0; i < nodes.num_rows; i++){
			if (nodes.time[i] == minTime){
				samples.push_back((node_id_t)i);
			}
		}
	*/

	//In nonWF, Use out IVAR to dig the samples from slim. 
	
		//std::cout << "IN NONWF CASE" << std::endl;	
		//std::vector<Subpopulation*> subpops = population_;

	//std::cout << population_.size() << std::endl;		
	
	for ( it = population_.begin(); it != population_.end(); it++){
		//std::cout << it->second->parent_individuals_.size() << std::endl;
		subpopulationIndividuals = it->second->parent_individuals_;
		populationIndividuals.insert(populationIndividuals.end(), subpopulationIndividuals.begin(), subpopulationIndividuals.end());
	}

//	std::cout << populationIndividuals.size() << std::endl;
	
	slim_pedigreeid_t IndID;
	int G1;
	int G2;
	int newValueInNodeTable = 0;
	for (unsigned i; i < populationIndividuals.size(); i++){
		IndID = populationIndividuals[i]->PedigreeID();
		G1 = 2 * IndID;
		G2 = G1 + 1;

		samples.push_back(getMSPID(G1));
		samples.push_back(getMSPID(G2));	
				
		newMap[G1] = (node_id_t)newValueInNodeTable++;
		newMap[G2] = (node_id_t)newValueInNodeTable++;
		//std::cout << "i:" << i << std::endl;	
		//std::cout << "indID: " << IndID << std::endl;
		//std::cout << "g1: " << G1 << std::endl;
		//std::cout << "g2: " << G2 << std::endl;
	}
				
	//node_table_print_state(&nodes,stdout);
	//edge_table_print_state(&edges,stdout);

	//Simplification through MSPrime API


	ret = sort_tables(&nodes, &edges, &migrations, &sites, &mutations, 0);				
	if (ret < 0) {
		handle_error("sort_tables", ret);
	}
	
	ret = simplifier_alloc(&simplifier, 100000.0, samples.data(), samples.size(),
	    &nodes, &edges, &migrations, &sites, &mutations, 0, 0);
	if (ret < 0) {
		handle_error("simplifier_alloc", ret);
	}

	ret = simplifier_run(&simplifier, NULL);
	if (ret < 0) {
		handle_error("simplifier_run", ret);
	}

	simplifier_free(&simplifier);
	
	//numberOfSamplesInLastSimplification = samples.size();
	samples.clear();	
	
	FSIDAS = (int)((CurrentTreeSequenceIndividual->PedigreeID()) * 2) + 2;
	FMIDAS = (int)nodes.num_rows;

	SLiM_MSP_Id_Map = newMap;	
	

}

node_id_t SLiMSim::getMSPID(int GenomeID){

	int Offset = GenomeID - FSIDAS;
	node_id_t retNode = (Offset < 0 ? (SLiM_MSP_Id_Map[GenomeID]) : (FMIDAS + Offset));
	return retNode;

}

void SLiMSim::StartTreeRecording(void)
{
	// Record any initial information needed about the simulation here.  Not sure what information you need.
	
	std::cout << "Starting tree sequence recording:" << std::endl;
	std::cout << "   Chromosome last base position: " << chromosome_.last_position_ << std::endl << std::endl;
	
	// This would also be the right place to allocate any storage you need, initialize ivars, etc.  This method will be called once,
	// immediately after the simulation finishes initializing (after all initialize() callbacks have completed).  It will not be called
	// again on this SLiMSim instance – but note that in SLiMgui multiple SLiMSim instances may exist, and may all be recording their
	// own trees, so your code needs to be capable of handling that.  Store your state inside SLiMSim, not in globals.  SLiMgui is
	// single-threaded, though, so you don't need to worry about re-entrancy or multithreading issues.


	/*
	WRITE TABLES TO A TEXT FILE	

	//open a file to to write text NODE and EDGE tables to test tree output
	MspTxtNodeFile.open("NodeTable.txt");
	MspTxtEdgeFile.open("EdgeTable.txt");

	if(MspTxtNodeFile.fail()){
		std::cout << "failed\n";
	}else{
		std::cout << "did not fail\n";
	}		
	MspTxtNodeFile << "is_sample\t" << "id\t" << "time\t" << "population\n";
	MspTxtEdgeFile << "left\t" << "right\t" << "parent\t" << "child\n";
	*/

	
	simplificationInterval = 10;
	FSIDAS = 0;
	FMIDAS = 0;
	//numberOfSamplesInLastSimplification = 0;
	lastSimplificationGeneration = 0;
		
	//INITIALIZE NODE AND EDGE TABLES.
	
	ret = node_table_alloc(&nodes, 0, 0);
    	if (ret != 0) {
		handle_error("alloc_nodes", ret);
		std::cout << "alloc nodes failed" << std::endl; 
    	}
    	ret = edge_table_alloc(&edges, 0);
    	if (ret != 0) {
		handle_error("alloc_edges", ret);
		std::cout << "alloc edged failed" << std::endl; 
    	}
    	//Even though we're not going to use them, we still have to allocate
     	//migration, site and mutations tables because the simplifier class
     	//expects them. This is annoying and will be fixed at some point 
    	ret = migration_table_alloc(&migrations, 0);
    	if (ret != 0) {
		handle_error("alloc_migrations", ret);
		std::cout << "alloc migrations failed" << std::endl; 
    	}
    	ret = site_table_alloc(&sites, 0, 0, 0);
    	if (ret != 0) {
		handle_error("alloc_sites", ret);
		std::cout << "alloc sites failed" << std::endl; 
    	}
    	ret = mutation_table_alloc(&mutations, 0, 0, 0);
    	if (ret != 0) {
		handle_error("alloc_mutations", ret);
		std::cout << "alloc mutations failed" << std::endl; 
    	}
		
	
	std::cout << "succesfully allocated tables" << std::endl;
			
	
}

void SLiMSim::RecordNewIndividual(Individual *p_individual)
{
	//THOUGHT: ALL THIS METHOD DOES IS SET IVARS, we could probably speed things up by setting the ivars instead of calling this method 

	// this is called by code where new individuals are created
	
	// The individual's pedigree ID should be set up already, as are its parents.  Parents can be -1, meaning that the individual started out
	// with empty genomes and has no parents (as when a new subpopulation is created).  Both parent pedigree ids can also be the same, which
	// presently indicates the result of *either* clonal reproduction or hermaphroditic selfing; no distinction is drawn between those in
	// the pedigree tracking code right now, but that could be changed (it would be logical for the second parent to be -1 for cloning, I
	// think).  The first parent is always the female in sexual models, guaranteed.  At present, when this code is called the individual may
	// not be completely initialized yet; it may not know its sex, and its genomes may not know their types, and so forth.  If that needs to
	// be fixed, it should be reasonably straightforward to do so.  For now, the only information guaranteed valid is the pedigree IDs.


	
	//DEBUG STDOUT PRINTING
	/*
	slim_pedigreeid_t ind_pid = p_individual->PedigreeID();
	slim_pedigreeid_t p1_pid = p_individual->Parent1PedigreeID();
	slim_pedigreeid_t p2_pid = p_individual->Parent2PedigreeID();
	
	std::cout << "--------------------------------------------------" << std::endl << std::endl;
		
	std::cout << Generation() << ": New individual created, pedigree id " << ind_pid << " (parents: " << p1_pid << ", " << p2_pid << ")" << std::endl << std::endl;
	*/


	//Set ivar to current individual, this way the calls to RecordRecombination have reference.
	CurrentTreeSequenceIndividual = p_individual;
	//Set ivar to indicate the first recombination has not been called, (this lets us know which parent each recombination is referring to)
	FirstRecombinationCalled = false;
		
}

void SLiMSim::RecordRecombination(std::vector<slim_position_t> *p_breakpoints, bool p_start_strand_2)
{
	// this is called by code where recombination occurs; it will not be called if recombination cannot occur, at present
	
	// Note that the breakpoints vector provided may (or may not) contain a breakpoint, as the final breakpoint in the vector, that is beyond
	// the end of the chromosome.  This is for bookkeeping in the crossover-mutation code and should be ignored, as the code below does.
	// The breakpoints vector may be nullptr (indicating no recombination), but if it exists it will be sorted in ascending order.

	//At the appropriate time, do simplification on tables	
	//This should be done at a higher level in SLiM for effeciency. 

	
	
/*	
	bool justSimplified = false;
	if ((int)(Generation() % simplificationInterval == 0) && ((int)Generation() != lastSimplificationGeneration)){
		simplifyTables();
		//std::cout << "in HERE" << Generation() << std::endl;
		justSimplified = true;
		lastSimplificationGeneration = (int)Generation();
	}
*/
	


	//BIOLOGY NOTE: Recombination is the meiosis process by which the parent gamete produces germ cells, hence two recombination events 
	//per diploid individual. Here each we treat each offspring genome produced as a haploid individual.

	slim_pedigreeid_t parentSLiMID;			//The SLiM gamete that produced this germ cell
	slim_pedigreeid_t offspringSLiMID;		//The SLiM germ cell being produced
	slim_pedigreeid_t genome1SLiMID;		//First genome ID of Gamete
	slim_pedigreeid_t genome2SLiMID;		//Second genome ID of Gamete  
	node_id_t offspringMSPID;			//MSPrime equivilent of germ cell ID (Node returned from MSPrime)
	node_id_t genome1MSPID;				//MSPrime equivilent of first genome ID of gemete
	node_id_t genome2MSPID;				//MSPrime equivilent of second genome ID of gemete

	
	//DEBUG STDOUT PRINTING
	/*	
	std::cout << Generation() << ":   Reference to individual: " << CurrentTreeSequenceIndividual->PedigreeID() << std::endl; 
	*/


	
	

 	//if the first recombination has not been called this is a reference to parent 1
	if(!FirstRecombinationCalled){ 			
		offspringSLiMID = 2 * CurrentTreeSequenceIndividual->PedigreeID();
		parentSLiMID = CurrentTreeSequenceIndividual->Parent1PedigreeID();
		FirstRecombinationCalled = true;		
	}else{
		offspringSLiMID = (2 * CurrentTreeSequenceIndividual->PedigreeID()) + 1;
		parentSLiMID = CurrentTreeSequenceIndividual->Parent2PedigreeID();
	} 

		
	//calculate genome ID's 
	genome1SLiMID = 2 * parentSLiMID;
	genome2SLiMID = 2 * parentSLiMID + 1;

	genome1MSPID = getMSPID(genome1SLiMID);
	genome2MSPID = getMSPID(genome2SLiMID);

	//add genome node
	double time = (double) -1 * Generation();
	uint32_t flags = 1;
	offspringMSPID = node_table_add_row(&nodes,flags,time,0,NULL,0);
	


	//once we get the call to simplifyTables() higher in SLiM, These should be updated in simplifyTables instead. 

/*	
	if(justSimplified){
		if(FSIDAS != (int)offspringSLiMID || FMIDAS != (int)offspringMSPID){
			std::cout << "wtf " << std::endl;
		}	
	}
*/	
	
	//THE FOLLOWING ONLY WORKS FOR WF, 
	//FOR NON_WF: The second case (FMIDAS + offset) works. but for all individuals that were sampled, we would need to keep a mapping SLiMID - > MSPID 
	

	//Map the Parental Genome SLiM Id's 

	/*
	int offset1 = (int)genome1SLiMID - FSIDAS;
	int offset2 = (int)genome2SLiMID - FSIDAS;
	genome1MSPID = (offset1 < 0 ? (node_id_t)(numberOfSamplesInLastSimplification + offset1) : (node_id_t)(FMIDAS + offset1));
	genome2MSPID = (offset2 < 0 ? (node_id_t)(numberOfSamplesInLastSimplification + offset2) : (node_id_t)(FMIDAS + offset2));
	*/
	


	
	//DEBUG STDOUT PRINTING 
	/*
	std::cout << Generation() << ":  Call to RecordRecombination for Genome: " <<  offspringSLiMID;
	std::cout << " (ParentID: " << parentSLiMID << ")" << std::endl;
	*/

	
	/*
	//WRITE TABLES TO TXT FILE
	
	MspTxtNodeFile << 1 << "\t" << genomeID << "\t" <<  -1 * Generation() <<" \t" << CurrentTreeSequenceIndividual->subpopulation_.subpopulation_id_ << "\n";
	*/


	
	

	size_t breakpoint_count = (p_breakpoints ? p_breakpoints->size() : 0);
	
	if (breakpoint_count && (p_breakpoints->back() == chromosome_.last_position_mutrun_ + 1))

		breakpoint_count--;
	
	if (breakpoint_count)
	{

		/*	
		//DEBUG STDOUT PRINTING	
		std::cout << Generation() << ":     Recombination at positions:";
		
		//This loop is just for printing our breakpoints
		for (size_t breakpoint_index = 0; breakpoint_index < breakpoint_count; ++breakpoint_index){
			std::cout << " " << (*p_breakpoints)[breakpoint_index];
		}
		std::cout << " (start with parental genome " << (p_start_strand_2 ? 2 : 1) << ")" << std::endl;
		

		//I want to add the beginning and end position of the chromosome to the vector so it's easier to compute edge intevals
		*/
		
		p_breakpoints->insert(p_breakpoints->begin(),0);
		p_breakpoints->pop_back();
		p_breakpoints->push_back(chromosome_.last_position_);
		

//		breakpoint_count++;


		/*
		//DEBUG STDOUT PRINTING
		for (int i = 0; i < p_breakpoints->size(); i ++){
			std::cout << "p_break[" << i << "]" << (*p_breakpoints)[i] << std::endl;
		}
		std::cout << std::endl;	
		*/
		
		//This loop computes all the edge intervals based upon breakpoints given
		for (size_t breakpoint_index = 0; breakpoint_index < breakpoint_count; ++breakpoint_index){
			
			//add edge
			double left = (double) (*p_breakpoints)[breakpoint_index];
			double right = (double) (*p_breakpoints)[breakpoint_index + 1];

			if (right <= left){
				continue;
//				std::cout << "YOOOO left = " << left << " & right = " << right << std::endl; 
//				handle_error("right <= left",-1);
			}

			node_id_t parent = (node_id_t) (p_start_strand_2 ? genome2MSPID : genome1MSPID);
			ret = edge_table_add_row(&edges,left,right,parent,offspringMSPID);
			if (ret < 0) {
				handle_error("add_edge", ret);
			}

		//DEBUG STDOUT PRINTING
/*		
			std::cout << Generation() << ":     ARGrecorder.AddEdge(left = ";
			std::cout << (*p_breakpoints)[breakpoint_index] << ",right = " << (*p_breakpoints)[breakpoint_index + 1];
			std::cout << ",parent = " << (p_start_strand_2 ? genome2MSPID : genome1MSPID) << ",child = " << offspringMSPID << ");" << std::endl;
*/			

			/*
			//WRITE TABLES TO A TEXT FILE
			
			MspTxtEdgeFile << (*p_breakpoints)[breakpoint_index] << "\t"; 
			MspTxtEdgeFile << (*p_breakpoints)[breakpoint_index + 1] << "\t"; 
			MspTxtEdgeFile << (p_start_strand_2 ? genome2SLiMID : genome1SLiMID ) << "\t" << genomeID << "\n";
			*/
			p_start_strand_2 = !p_start_strand_2;
	
		}

	}
	else if (p_breakpoints)
	{

		
	
		/*
		//DEBUG STDOUT PRINTING 

		std::cout << Generation() << ":     No recombination (use parental strand " << (p_start_strand_2 ? 2 : 1) << ")" << std::endl;
		std::cout << Generation() << ":     ARGrecorder.AddEdge(left = " << 0 << ",right = " << chromosome_.last_position_;
		std::cout << ",parent = " << (p_start_strand_2 ? genome2SLiMID : genome1SLiMID) << ",child = " << genomeID << ");" << std::endl;
		*/

	
	//GOODSTUFF

		double left = 0;
		double right = (double) chromosome_.last_position_;
		node_id_t parent = (node_id_t) (p_start_strand_2 ? genome2MSPID : genome1MSPID);
		ret = edge_table_add_row(&edges,left,right,parent,offspringMSPID);
		if (ret < 0) {
			handle_error("add_edge", ret);
		}

	
	
		/*
		//WRITE TABLES TO A TEXT FILE
		
		MspTxtEdgeFile << 0 << "\t" << chromosome_.last_position_ << "\t";
		MspTxtEdgeFile << (p_start_strand_2 ? genome2SLiMID : genome1SLiMID)<< "\t" << genomeID << "\n";
		*/
	
	
	//GOODSTUFF

	}
	else
	{

	
	
		/*
		//DEBUG STDOUT PRINTING		

		std::cout << Generation() << ":     No parental genome" << std::endl;


		*/
	
	
	//GOODSTUFF
	}
	

	//std::cout << std::endl;

}

void SLiMSim::WriteTreeSequence(void)
{
	// Write the recorded tree sequence stuff to recording_tree_path_; see Eidos_ExecuteFunction_writeFile()
	// for some example file-writing code, but I guess you'll maybe be using that library you mentioned.
	// In the present design this method is called only by SLiMSim::SimulationFinished(), so the run is over
	// and recording is done.  In future I could imagine allowing the user to write out a tree file at multiple
	// time points, though, so it might be good to postpone cleanup and freeing of resources until ~SLiMSim().
	
	std::cout << Generation() << ": ***** Writing tree sequence file to path " << recording_tree_path_ << std::endl;

	//node_table_print_state(&nodes,stdout);
	//edge_table_print_state(&edges,stdout);
	simplifyTables();
	node_table_print_state(&nodes,stdout);
	edge_table_print_state(&edges,stdout);

	
	//freeing these for now. Will change this after I get writing figured out.
	//simplifier_free(&simplifier);
    	node_table_free(&nodes);
    	edge_table_free(&edges);
    	migration_table_free(&migrations);
    	site_table_free(&sites);
    	mutation_table_free(&mutations);

	/*
	WRITE TABLES TO A TEXT FILE
	
	MspTxtNodeFile.close();
	MspTxtEdgeFile.close();
	*/
	
}

