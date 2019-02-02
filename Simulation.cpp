#include "Simulation.h"


Simulation::Simulation(SchedulerAlgorithm& algorithm, double simulationTime, int nodeNumber)
	: scheduler_{algorithm}, simulationTime{simulationTime}
{
	for (int i = 0; i < nodeNumber; i++)
		nodes_.emplace_back();
}


Simulation::~Simulation() = default;

void Simulation::simulate() 
{

	if (simulationTime == 0) 
	{
		cout << "Simulation End. Reason : Simulation Time Interval is 0" << endl;
	}

	logger.clearLog();
	//vQ.flush();
	//vPQ.flush();


	//	cout<< "1" << endl;
	//if (taskPool_.getTaskSource() == Random) {
		taskPool_.flush();
		taskPool_.fill(simulationTime);
	//}
	//else {
	//	taskPool_.reloadTaskSeeker();
	//	taskPool_.resettingtaskPool_(simulationTime);
	//}

	//	cout<< "2 " << endl;


	for (auto& node : nodes_)
		node.set_current_time(taskPool_.getSimulationStartTime());


	//	cout << "3" << endl;

		//���� �� �� ������ �½�ũ�� ���� �����ٷ� ����
	double nextAT = taskPool_.getTime(); //nextArrivalTime

//	cout<< "4" << endl;

	Task* arrived_task = &taskPool_.dequeue_task();


	//	cout<< "5" << endl;
	printArrive(*arrived_task);
	//	cout<< "6" << endl;
	run_scheduler(nextAT, *arrived_task);
	//	cout<< "7" << endl;


		//���� �ùķ����� ������� ��� �����ٷ��� CPU�� �����Ѵ�.
	while (!taskPool_.isEnd()) {
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
					node.execute(nextAT);
					node.set_arrival_time_reached(true);
				}
				else {
					//if (completeSignalOn) {
					//	//���࿡ �Ϸ� ��ȣ�� ���������� �ӽ÷� �����Ų��.
					//	completeTime[i] = vCore.getCoreTime(i) + vCore.getLeftTime(i);
					//}
					/*else {*/
						//�Ϸ��ȣ�� ���� ���� ��������� �����Ų��.
						node.execute(nextAT);
					/*}*/
				}
			}
			

			//if (completeSignalOn) {
			//	double smallestTime = completeTime[7];

			//	for (int i = 0; i < 8; i++) {
			//		if (completeTime[i] < smallestTime) {
			//			smallestTime = completeTime[i];
			//		}
			//	}

			//	//���� ���� �ð����� ��� �ھ ����
			//	for (int i = 0; i < 8; i++) {
			//		vCore.execute(i, smallestTime);
			//	}

			//	//�Ϸ� �����췯�� �����Ѵ�.
			//	if (nextAT != smallestTime) {
			//		cScheduler(smallestTime);
			//	}
			//}

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
			node.set_arrival_time_reached = false;
		}
	}
	for (auto& node : nodes_) {
		nodes_= 
	}

	//���⿡ �ùķ��̼� ������� �۵��ϴ� �ڵ带 ����־���մϴ�.
	//����� Task Pool�� ��� �������� �ٷ� ����Ǵ� �����̹Ƿ�
	//Task ���� - �ùķ��̼� ��������� �̹��� �ð��� ������� �ʰ� �ֽ��ϴ�.

	//vCore.failAllTask(taskPool.getSimulationEndTime());

	//for (int i = 0; i < 8; i++) {
	//	unsigned int size = vPQ.size(i);
	//	Task temp;

	//	for (unsigned int j = 0; j < size; j++) {

	//		vPQ.deQueue(i, temp);

	//		temp.finishTime = taskPool.getSimulationEndTime();
	//		temp.startTime = taskPool.getSimulationEndTime();
	//		temp.state = Fail;

	//		logger.writeLog(i, &temp);
	//	}
	//}

	//for (int i = 0; i < 8; i++) {
	//	unsigned int size = vQ.size(i);
	//	for (unsigned int j = 0; j < size; j++) {
	//		Task temp;

	//		vQ.deQueue(i, temp);

	//		temp.core = 8;
	//		temp.finishTime = 0;
	//		temp.startTime = 0;
	//		temp.state = Drop;

	//		logger.writeLog(8, &temp);
	//	}
	//}



	logger.createLogFile(scheduler_.get_algorithm_name());
}

//const Task& Simulation::get_arrived_task() const
//{
//	return arrived_task_;
//}

//void Simulation::set_arrived_task(Task task)
//{
//	arrived_task_ = task;
//}

void Simulation::run_scheduler(double currentTime, Task& arrivedTask)
{
	scheduler_.run(currentTime, nodes_, arrivedTask);
}

