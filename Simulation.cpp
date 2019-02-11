#include "Simulation.h"
#include "RandomAlgorithm.h"
#include "MCTAlgorithm.h"


Simulation::Simulation(SchedulerAlgorithm& algorithm, double simulationTime, int nodeNumber)
	: simulationTime{simulationTime}
{
	for (int i = 0; i < nodeNumber; i++)
		nodes_.emplace_back();
}


Simulation::~Simulation() = default;

void Simulation::simulate()
{
	int input = 0;

	print_intro();

	while (true)
	{
	
		print_menu(MAIN);
		cin >> input;
		
		if (input == 1)
		{
			cout << "setting environment" << endl;
			set_simulation_environment();
		}
		else if (input == 2)
		{
			cout << "running simulation" << endl;

			if (simulationTime == 0)
			{
				cout << "Simulation End. Reason : Simulation Time Interval is 0" << endl;
			}

			logger_.clearLog();
	
			taskPool_.flush();
			taskPool_.fill(simulationTime);
			
			for (auto& node : nodes_)
				node.set_current_time(taskPool_.getSimulationStartTime());

			double nextAT = taskPool_.getTime(); //nextArrivalTime

			Task* arrived_task = &taskPool_.dequeue_task();

			printArrive(*arrived_task);
			run_scheduler(nextAT, *arrived_task);

			int count = 0;

			//���� �ùķ����� ������� ��� �����ٷ��� CPU�� �����Ѵ�.
			while (!taskPool_.isEnd()) {

				count++;

				nextAT = taskPool_.getTime();

				bool allCoreLeachAT = false;

				while (!allCoreLeachAT) {

					//�ھ �۾��� �ε�
					for (auto& node : nodes_)
					{
						while (node.get_state() == NodeState::Idle)
						{
							if (!node.try_set_paused_task() && !node.try_set_task_from_queue())
							{
								//�� �۾��� ���� -> ���� �۾� �������� ���
								node.set_arrival_time_reached(true);
								break;
							}

						}
					}

					//double completeTime[8] = { nextAT,nextAT,nextAT,nextAT,nextAT,nextAT,nextAT,nextAT };

					//����
					for (auto& node : nodes_)
					{

						if (node.is_arrival_time_reached()) {
							//�� �۾��� ����
						}
						else if (node.get_current_time() == nextAT) {
							//�̹� ���� �۾��� ���� �ð����� ����Ϸ�
							node.set_arrival_time_reached(true);
						}
						else if (nextAT < node.get_current_time() + node.get_left_time()) {
							//���� �Ϸ�ð��� ���� �۾� �����ð��� �ѱ�� �����ð������� ����
							node.execute(nextAT, logger_);
							node.set_arrival_time_reached(true);
						}
						else {
						
							node.execute(nextAT, logger_);
							/*}*/
						}
					}


				

					//��� �ھ ���� �۾� �����ð��� �Դ��� üũ.
					allCoreLeachAT = true;
					for (auto& node : nodes_) {
						allCoreLeachAT &= node.is_arrival_time_reached();
					}
				}

				//��� �ھ ���� �۾��� ������ �ð����� �����Ƿ� ���� �۾��� ���� �����ٷ� ����.

				arrived_task = &taskPool_.dequeue_task();
				printArrive(*arrived_task);
				run_scheduler(nextAT, *arrived_task);

				// Reset arrival indicators
				for (auto& node : nodes_)
				{
					node.set_arrival_time_reached(false);
				}
			}

			logger_.createLogFile(scheduler_.get_algorithm_name());

			print_result();
		}
		else if (input == 3)
		{
			cout << "end simulation." << endl;
			return;
		}
	}

}


void Simulation::run_scheduler(double currentTime, Task& arrivedTask)
{
	scheduler_.run(currentTime, nodes_, arrivedTask);
}
void Simulation::print_intro()
{
	system("cls");

	cout << "---------------------------------------------------------------------" << endl;
	cout << "                   Welcome to The Server Simulator                   " << endl;
	cout << "---------------------------------------------------------------------" << endl;
	cout << endl;
}

void Simulation::print_menu(int select)
{

	switch (select)
	{
	case MAIN:
		cout << "Menu" << endl;
		cout << "1. Set simulation environment" << endl;
		cout << "2. Run simulation" << endl;
		cout << "3. End simulation" << endl;
		cout << endl;

		cout << "Select Menu : ";
		break;
	case SET_ENV:
		cout << "Menu" << endl;
		cout << "1. Set simulation time" << endl;
		cout << "2. Set core setting" << endl;
		cout << "3. Set Algorithm" << endl;
		cout << "4. End setting environment" << endl;
		cout << endl;
		break;
	case SET_CORE:
		break;
	case SET_ALGO:
		cout << "Algorithm Selection" << endl;
		cout << "1. random" << endl;
		cout << "2. MCT" << endl;
		cout << "3. algorithm 3" << endl;
		cout << "4. algorithm 4" << endl;
		cout << "5. algoirhtm 5 " << endl;
		cout << endl;

		break;
	}
}


void Simulation::set_simulation_environment() 
{
	int input = 0;
	int assignedNode = 0;
	int temp = 0;
	int coreType = 0;
	int algo = 0;

	while (1)
	{
		print_menu(SET_ENV);
		cin >> input;



		switch (input)
		{
		case SET_TIME:
			cout << "Set Simulation time : ";
			cin >> simulationTime;
			cout << "Simulation time set to : " << simulationTime << endl;
			break;
		case SET_CORE:

			nodes_.clear();
			
			cout << "Set Number of cores : ";
			cin >> numNode;
			
			cout << assignedNode << " nodes are assigned core numbers out of " << numNode << endl;
			cout << endl;

			while (assignedNode < numNode)
			{
				cout << "number of cores (maximum " << numNode - assignedNode << ") : ";
				cin >> temp;
				cout << "Select node type for " << temp << " cores (1-2) : ";
				cin >> coreType;
				for (int ii = assignedNode; ii < assignedNode + temp; ii++)
				{
					// create nodes and assign node types
					// node creator
					// set node type
					nodes_.emplace_back(coreType);
					cout << assignedNode << " ";
				}
				cout << endl;
				cout << temp << " cores are set to be type " << coreType << endl;;
				assignedNode += temp;
			}

			// TODO: types
			for (int i = 0; i < assignedNode; i++)
				nodes_.emplace_back();

			break;
		case SET_ALGO:
			print_menu(SET_ALGO);
			cin >> algo;
			// algo ���� �ʿ��� ���� �����ؼ� �װɷ� �˰����� ��������
			switch (algo)
			{
			case 1:
				cout << "scheduler set to use random algorithm" << endl;
				scheduler_.change_algorithm(new RandomAlgorithm);
				break;
			case 2:
				cout << "scheduler set to use MCT algorithm" << endl;
				scheduler_.change_algorithm(new MCTAlgorithm);
				break;
			case 3:
				cout << "scheduler set to use algorithm 3" << endl;

				break;
			case 4:
				cout << "scheduler set to use algorithm 4" << endl;

				break;
			case 5:
				cout << "scheduler set to use algorithm 5" << endl;

				break;
			}
			break;
		case 4:
			cout << "Finished setting envirionment" << endl;
			return;
			break;
		}

	}
}
void Simulation::print_result()
{
	cout << "Printing result" << endl;
	
	cout << "make span" << endl;
	cout << "execution success rate " << endl;
	cout << "cput utilization " << endl;
	// �ٸ� ��Ÿ��� ���
	
	cout << endl;


}