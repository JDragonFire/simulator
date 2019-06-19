using System.Text;

namespace ScenarioPreprocessor
{
    public class ScenarioEntry
    {
        public string event_target;
        public string event_action;
        public long event_timestamp;
        public EventDetail event_detail;

        public bool MultiHost;

        public struct EventDetail
        {
            public string host_name;
            public string host_status;
            public string cluster_name;
            public string queue_name;
            public string exec_hostname;
            public string application_name;
            public long job_id;
            public long mem_req;
            public int number_of_jobs;
            public double num_exec_procs;
            public int num_slots;
            public long job_mem_usage;
            public long job_swap_usage;
            public double job_cpu_time;
            public double job_run_time;
            public string job_exit_status;
            public int job_exit_code;
            public double job_non_cpu_time;
        }

        public override string ToString()
        {
            var sb = new StringBuilder();
            bool isSubmission = event_detail.host_name == null;
            sb.AppendLine($"# Timestamp : {event_timestamp}");
            if (isSubmission)
            {
                sb.AppendLine($"# Type: submission");
                sb.AppendLine($"# Queue Name: " + event_detail.queue_name);
                sb.AppendLine($"# Executed Host Name : " + event_detail.exec_hostname);
                sb.AppendLine($"# Application Name : " + event_detail.application_name);
                sb.AppendLine($"# Job Id : {event_detail.job_id}");
                sb.AppendLine($"# Memory Requirement : {event_detail.mem_req}");
                sb.AppendLine($"# Number of Jobs : {event_detail.number_of_jobs}");
                sb.AppendLine($"# Number of Executed Processes : {event_detail.num_exec_procs}");
                sb.AppendLine($"# Number of Slots : {event_detail.num_slots}");
                sb.AppendLine($"# Job Memory Usage : {event_detail.job_mem_usage}");
                sb.AppendLine($"# Job Run Time : {event_detail.job_run_time}");
                sb.AppendLine($"# Job CPU Time : {event_detail.job_cpu_time}");
                sb.AppendLine($"# Job Non CPU Time : {event_detail.job_non_cpu_time}");
                sb.AppendLine($"# Job Exit Status : {event_detail.job_exit_status}");
                sb.AppendLine($"# Job Exit Code : {event_detail.job_exit_code}");
                return sb.ToString();
            }
            sb.AppendLine($"# Type: Change Status");
            sb.AppendLine($"# Host Name: {event_detail.host_name}");
            sb.AppendLine($"# Host Status: {event_detail.host_status}");
            return sb.ToString();
        }
    }
}