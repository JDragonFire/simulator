using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Newtonsoft.Json;

namespace ScenarioPreprocessor
{
    class Program
    {
        public const string FOLDER_PATH = @"D:\OneDrive\Documents\HPIC\Scheduler";
        public const string SCNEARIO = @"scenario.json";
        public const string INITIAL_STATUS = @"hardware_raw_tracker_initial_status.json";

        static void Main(string[] args)
        {
            Dictionary<string, string> initialStatus;

            using (var fileStream = File.OpenRead(Path.Combine(FOLDER_PATH, INITIAL_STATUS)))
            {
                var tReader = new StreamReader(fileStream);
                string file = tReader.ReadToEnd();
                initialStatus = JsonConvert.DeserializeObject<Dictionary<string, string>>(file);
            }

            using (var fileStream = File.OpenRead(Path.Combine(FOLDER_PATH, SCNEARIO)))
            {
                var tReader = new StreamReader(fileStream);

                //for (int i = 0; i < 100; i++)
                //    Console.WriteLine(tReader.ReadLine());

                const int N = 100;
                var lines = new string[N];
                for (int i = 0; i < N; i++)
                    lines[i] = tReader.ReadLine();
                File.WriteAllLines(Path.Combine(FOLDER_PATH, "lines.json"), lines);


                //JsonTextReader reader = new JsonTextReader(tReader);

                //for (int i = 0; i < 10; i++)
                //{
                //    reader.Read();
                //    List<Scen>
                //}

                List<ScenarioEntry> list = new List<ScenarioEntry>();

                while (!tReader.EndOfStream)
                {
                    var entry = JsonConvert.DeserializeObject<ScenarioEntry>(tReader.ReadLine());

                    if (entry.event_action != "submission")
                        ;

                    list.Add(entry);
                }

                ;

                ;
            }

        }
    }

    public class ScenarioEntry
    {
        public string event_target;
        public string event_action;
        public long event_timestamp;
        public EventDetail event_detail;

        public class EventDetail
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
            public int num_exec_procs;
            public int num_slots;
            public long job_mem_usage;
            public long job_swap_usage;
            public double job_cpu_time;
            public double job_run_time;
            public string job_exit_status;
            public int job_exit_code;
            public double job_non_cpu_time;
        }
    }
}
