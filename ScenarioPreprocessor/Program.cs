using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.IO;
using Newtonsoft.Json;
using Elasticsearch.Net;
using Nest;
using Newtonsoft.Json.Converters;
using Newtonsoft.Json.Linq;

namespace ScenarioPreprocessor
{
    class Program
    {
        //public const string FOLDER_PATH = @"D:\OneDrive\Documents\HPIC\Scheduler";
        public const string FOLDER_PATH = @"../../../scenarios/";
        public const string SCENARIO_SLICE = "Scenario_From_1000000_1000000.json";
        public const string SCNEARIO = @"scenario.json";
        public const string INITIAL_STATUS = @"hardware_raw_tracker_initial_status.json";

        static void Main(string[] args)
        {
            //MergeMultiHostJobs();
            //AnalyseScenario();
            SliceScenario();
        }

        public static void SliceScenario(int count = 10000)
        {
            Console.WriteLine("Reading Scenario...");
            List<ScenarioEntry> list = new List<ScenarioEntry>(count);
            using(var fStream = File.OpenRead(Path.Combine(FOLDER_PATH, "merged_" + SCENARIO_SLICE)))
            {
                int i = 0;
                var sReader = new StreamReader(fStream);
                while (!sReader.EndOfStream && i < count)
                {
                    var jsonString = sReader.ReadLine();
                    try
                    {
                        var entry = JsonConvert.DeserializeObject<ScenarioEntry>(jsonString);
                        list.Add(entry);
                        i++;
                    }
                    catch (Exception e)
                    {
                        break;
                    }
                }
            }

            Console.WriteLine("Writing json file......");
            using (var fileStream = File.OpenWrite(Path.Combine(FOLDER_PATH, $"sliced_{count}_" + SCENARIO_SLICE)))
            {
                var writer = new StreamWriter(fileStream);

                for (int i = 0; i < list.Count; i++)
                    writer.WriteLine(JsonConvert.SerializeObject(list[i], Formatting.None));
            }
        }

        public static void MergeMultiHostJobs()
        {
            Console.WriteLine("Reading Scenario...");
            List<ScenarioEntry> list = new List<ScenarioEntry>(1_000_000);
            using(var fStream = File.OpenRead(Path.Combine(FOLDER_PATH, SCENARIO_SLICE)))
            {
                var sReader = new StreamReader(fStream);
                while (!sReader.EndOfStream)
                {
                    var jsonString = sReader.ReadLine();
                    try
                    {
                        var entry = JsonConvert.DeserializeObject<ScenarioEntry>(jsonString);
                        list.Add(entry);
                    }
                    catch (Exception e)
                    {
                        break;
                    }
                }
            }

            ScenarioEntry[][] grouped = list
                .GroupBy(e => new
                {
                    e.event_detail.job_id,
                    e.event_timestamp
                }).Select(g => g.ToArray()).ToArray();

            List<ScenarioEntry> newList = new List<ScenarioEntry>(list.Count);

            foreach (var group in grouped)
            {
                // Only consider groups
                // Pass 'Change Status' entries.
                if (group.Length == 1 || group[0].event_detail.host_name != null)
                {
                    newList.AddRange(group);
                    continue;
                }

                double procs = group[0].event_detail.num_exec_procs;

                // Pass procs == 1 groups
                if (procs == 1)
                {
                    newList.AddRange(group);
                    continue;
                }

                ScenarioEntry representative = group[0];
                representative.MultiHost = true;
                newList.Add(representative);

                Console.WriteLine($"{group.Length} of multi-host jobs are merged into 1.");
            }

            Console.WriteLine();
            Console.WriteLine("Writing json file......");
            using (var fileStream = File.OpenWrite(Path.Combine(FOLDER_PATH, "merged_" + SCENARIO_SLICE)))
            {
                var writer = new StreamWriter(fileStream);

                for (int i = 0; i < newList.Count; i++)
                    writer.WriteLine(JsonConvert.SerializeObject(newList[i], Formatting.None));
            }

            return;


            //// Find valid groups
            //{
            //    ScenarioEntry[][] groups = grouped.Where(p => p.Length > 1 && !p[0].event_action.StartsWith("ch")).ToArray();

            //    int count = 0;
            //    foreach (ScenarioEntry[] group in groups)
            //    {
            //        count++;

            //        double procs = group[0].event_detail.num_exec_procs;
            //        //double sum = 0.0;
            //        //bool flag = true;
            //        //for (int i = 0; i < group.Length; i++)
            //        //{
            //        //    if (procs == group[i].event_detail.num_slots)
            //        //    {
                            
            //        //    }
            //        //}

            //        Console.WriteLine($"### Group {count}: ------------------------");
            //        foreach (ScenarioEntry entry in group)
            //        {
            //            Console.WriteLine(entry);
            //        }

            //        Console.WriteLine();

            //        var key = Console.ReadKey();
            //        switch (key.Key)
            //        {
            //            case ConsoleKey.Escape:
            //                return;
            //            default:
            //                continue;
            //        }

            //        end: ;
            //    }
            //}
        }

        public static void AnalyseScenario()
        {
            Console.WriteLine("Reading hosts......");
            string hostJson = File.ReadAllText(Path.Combine(FOLDER_PATH,
                "host.json"));
            Dictionary<string, StaticHostInfo> staticHostInfo
                = JsonConvert.DeserializeObject<Dictionary<string, StaticHostInfo>>(hostJson);

            Console.WriteLine("Reading scenarios...");
            //List<ScenarioEntry> list = new List<ScenarioEntry>();
            Dictionary<string, int> statuses = new Dictionary<string, int>();
            List<ScenarioEntry> scenario = new List<ScenarioEntry>();
            using(var fStream = File.OpenRead(Path.Combine(FOLDER_PATH, "Scenario_From_1000000_1000000.json")))
            {
                var sReader = new StreamReader(fStream);
                while (!sReader.EndOfStream)
                {
                    var jsonString = sReader.ReadLine();
                    try
                    {
                        var entry = JsonConvert.DeserializeObject<ScenarioEntry>(jsonString);
                        scenario.Add(entry);
                    }
                    catch
                    {
                        break;
                    }


                    //if (entry.event_detail.job_exit_status == "EXIT")
                    //    Console.WriteLine(entry.ToString());

                    //if (entry.event_detail.num_exec_procs != entry.event_detail.num_slots)
                    //{
                    //    Console.WriteLine(entry.ToString());
                    //    Console.WriteLine("##########################");
                    //    Console.WriteLine(staticHostInfo[entry.event_detail.exec_hostname]);
                    //    Console.WriteLine("---------------------------------------------------");
                    //    Console.ReadKey();
                    //}

                    //var key = entry.event_detail.job_exit_status;
                    //if (key == null) continue;
                    //if (statuses.ContainsKey(key))
                    //    statuses[key]++;
                    //else
                    //    statuses[key] = 1;
                }
            }

            var grouped = scenario.GroupBy(e => e.event_timestamp);
            foreach (var group in grouped)
            {
                foreach (var entry in group)
                {
                    Console.WriteLine(entry.ToString());
                }
                Console.WriteLine("##########################");
                Console.WriteLine(staticHostInfo[group.First().event_detail.exec_hostname]);
                Console.WriteLine("---------------------------------------------------");
                Console.ReadKey();
            }

            //foreach (var item in statuses)
            //{
            //    Console.WriteLine($"{item.Key} : {item.Value}");
            //}
        }

        public static void ExtractScenario()
        {
            const int OFFSET = 1000000;
            const int COUNT = 1000000;
            Dictionary<string, string> hostStatuses;
            List<ScenarioEntry> list = new List<ScenarioEntry>(OFFSET + COUNT);

            {
                string status = File.ReadAllText(Path.Combine(FOLDER_PATH, INITIAL_STATUS));
                hostStatuses = JsonConvert.DeserializeObject<Dictionary<string, string>>(status);
            }

            using (var fileStream = File.OpenRead(Path.Combine(FOLDER_PATH, SCNEARIO)))
            {
                var tReader = new StreamReader(fileStream);
                var converter = new ScenarioConverter();
                int count = 0;
                bool flag = false;

                while (!tReader.EndOfStream)
                {
                    string line = tReader.ReadLine();
                    //ScenarioEntry entry = JsonConvert.DeserializeObject<ScenarioEntry>(line, converter);
                    ScenarioEntry entry = JsonConvert.DeserializeObject<ScenarioEntry>(line);

                    list.Add(entry);
                    count++;
                    if (count == OFFSET + COUNT)
                        break;
                }
            }

            var result = list
                .OrderBy(e => e.event_timestamp)
                .GroupBy(e => new
                {
                    e.event_detail.job_id,
                    e.event_timestamp,
                })
                .Where(g => g.Count() > 1)
                .Select(g => g.ToArray())
                .ToArray();



            for (int i = 0; i < OFFSET; i++)
            {
                if (list[i].event_action.StartsWith("ch"))
                {
                    hostStatuses[list[i].event_detail.host_name] = list[i].event_detail.host_status;
                }
            }

            using (var fileStream = File.OpenWrite(Path.Combine(FOLDER_PATH, $"Scenario_From_{OFFSET}_{COUNT}.json")))
            {
                var writer = new StreamWriter(fileStream);

                for (int i = OFFSET; i < OFFSET + COUNT; i++)
                    writer.WriteLine(JsonConvert.SerializeObject(list[i], Formatting.None));
            }

            File.WriteAllText(Path.Combine(FOLDER_PATH, $"Scenario_From_{OFFSET}_{COUNT}_Status.json"),
                JsonConvert.SerializeObject(hostStatuses, Formatting.None));
        }

        public static SortedDictionary<string, StaticHostInfo> ElasticSearch()
        {
            var uri = new Uri("http://163.152.175.80:9200");

            var settings = new ConnectionSettings(uri)
                .DefaultMappingFor<RptHardwareRawEntry>(m => m
                    .TypeName("_doc")
                )
                .DefaultIndex("rpt_hardware_raw");

            var hClient = new ElasticClient(settings);

            var searchResponse = hClient.Search<RptHardwareRawEntry>(s => s
                .Size(2000)
                .Query(q => q
                    .Match(m => m
                        .Field(f => f.CLUSTER_NAME == "cr1")))
                .Scroll("5m")
            );

            //var hostNames = new Dictionary<string, RptHardwareRawEntry>();

            var hostInfos = new SortedDictionary<string, StaticHostInfo>();

            var results = hClient.Scroll<RptHardwareRawEntry>("10m", searchResponse.ScrollId);
            int count = 0;
            while (results.Documents.Any())
            {
                Console.WriteLine("2000 documents are scrolled.");

                foreach (var doc in results.Documents)
                {
                    if (!hostInfos.TryGetValue(doc.HOST_NAME, out var current))
                    {
                        hostInfos.Add(doc.HOST_NAME, StaticHostInfo.FromRaw(doc));
                    }
                    else
                    {
                        var temp = StaticHostInfo.FromRaw(doc);
                        hostInfos[doc.HOST_NAME] = StaticHostInfo.Compare(temp, current);
                    }
                }

                results = hClient.Scroll<RptHardwareRawEntry>("10m", results.ScrollId);
            }

            //var searchResponse = hClient.Search<dynamic>(s => s
            //    .AllTypes()
            //    .MatchAll());

            // var converter = JsonConvert.

            //searchResponse.Documents.Cast<JObject>().Select

            var json = JsonConvert.SerializeObject(hostInfos, Formatting.Indented);
            File.WriteAllText(Path.Combine(FOLDER_PATH, "host.json"), json);
            
            return hostInfos;
        }

        public static void CombineHost()
        {
            Console.WriteLine("Reading statuses...");

            string statusJson = File.ReadAllText(Path.Combine(FOLDER_PATH,
                "Scenario_From_1000000_1000000_Status.json"));

            Dictionary<string, string> status
                = JsonConvert.DeserializeObject<Dictionary<string, string>>(statusJson);

            Console.WriteLine("Reading hosts...");

            string hostJson = File.ReadAllText(Path.Combine(FOLDER_PATH,
                "host.json"));

            Dictionary<string, StaticHostInfo> staticHostInfo
                = JsonConvert.DeserializeObject<Dictionary<string, StaticHostInfo>>(hostJson);

            Console.WriteLine("Combining...");

            Dictionary<string, StaticHostInfo> result = new Dictionary<string, StaticHostInfo>(status.Count);

            foreach (var item in status)
            {
                var info = staticHostInfo[item.Key];
                info.STATUS = item.Value;
                result.Add(item.Key, info);
            }

            File.WriteAllText(Path.Combine(FOLDER_PATH, "hosts_final.json"),
                JsonConvert.SerializeObject(result, Formatting.Indented));
        }

        public static void GenerateQueueJson()
        {
            var uri = new Uri("http://163.152.175.80:9200");

            var settings = new ConnectionSettings(uri)
                .DefaultMappingFor<RptHardwareRawEntry>(m => m
                    .TypeName("_doc")
                )
                .DefaultIndex("lsf_bqueues");

            var hClient = new ElasticClient(settings);

            var searchResponse = hClient.Search<RptHardwareRawEntry>(s => s
                .Size(2000)
                .Query(q => q
                    .Match(m => m
                        .Field(f => f.CLUSTER_NAME == "cr1")))
                .Scroll("5m")
            );

            //var hostNames = new Dictionary<string, RptHardwareRawEntry>();

            var hostInfos = new SortedDictionary<string, StaticHostInfo>();

            var results = hClient.Scroll<RptHardwareRawEntry>("10m", searchResponse.ScrollId);
            int count = 0;
            while (results.Documents.Any())
            {
                Console.WriteLine("2000 documents are scrolled.");

                foreach (var doc in results.Documents)
                {
                    if (!hostInfos.TryGetValue(doc.HOST_NAME, out var current))
                    {
                        hostInfos.Add(doc.HOST_NAME, StaticHostInfo.FromRaw(doc));
                    }
                    else
                    {
                        var temp = StaticHostInfo.FromRaw(doc);
                        hostInfos[doc.HOST_NAME] = StaticHostInfo.Compare(temp, current);
                    }
                }

                results = hClient.Scroll<RptHardwareRawEntry>("10m", results.ScrollId);
            }

            //var searchResponse = hClient.Search<dynamic>(s => s
            //    .AllTypes()
            //    .MatchAll());

            // var converter = JsonConvert.

            //searchResponse.Documents.Cast<JObject>().Select

            var json = JsonConvert.SerializeObject(hostInfos, Formatting.Indented);
            File.WriteAllText(Path.Combine(FOLDER_PATH, "host.json"), json);

            return;
        }
    }

    public class ScenarioConverter : JsonConverter<ScenarioEntry>
    {
        #region Overrides of JsonConverter<ScenarioEntry>

        public override void WriteJson(JsonWriter writer, ScenarioEntry value, JsonSerializer serializer)
        {
            throw new NotImplementedException();
        }

        public override ScenarioEntry ReadJson(JsonReader reader, Type objectType, ScenarioEntry existingValue, bool hasExistingValue,
            JsonSerializer serializer)
        {
            JToken token = JToken.Load(reader);

            JToken target = token["event_target"];
            JToken detailToken = token["event_detail"];
            if (target.ToString()[0] == 'h')
            {
                var entry = new ChangeStatusEntry
                {
                    event_timestamp = token["event_timestamp"].Value<long>(),
                    event_detail = detailToken.ToObject<ChangeStatusEntry.ChangeStatusDetail>()
                };

                return entry;
            }
            else
            {
                var entry = new SubmissionEntry();
                var properties = detailToken.Cast<JProperty>().ToArray();
                entry.event_detail = new SubmissionEntry.SubmissionDetail
                {
                    cluster_name = properties[0].Value.ToString(),
                    queue_name = properties[1].Value.ToString(),
                    exec_hostname = properties[2].Value.ToString(),
                    application_name = properties[3].Value.ToString(),
                    job_id = properties[4].Value.ToObject<long>(),
                    mem_req = properties[5].Value.ToObject<long>(),
                    number_of_jobs = properties[6].Value.ToObject<int>(),
                    num_exec_procs = properties[7].Value.ToObject<double>(),
                    num_slots = properties[8].Value.ToObject<int>(),
                    job_mem_usage = properties[9].Value.ToObject<long>(),
                    job_swap_usage = properties[10].Value.ToObject<long>(),
                    job_cpu_time = properties[11].Value.ToObject<double>(),
                    job_run_time = properties[12].Value.ToObject<double>(),
                    job_exit_status = properties[13].Value.ToString(),
                    job_exit_code = properties[14].Value.ToObject<int>(),
                    job_non_cpu_time = properties[15].Value.ToObject<double>()
                };

                return entry;
            }
        }

        #endregion
    }

    public class SubmissionEntry : ScenarioEntry
    {
        public SubmissionDetail event_detail;

        public struct SubmissionDetail
        {
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
    }

    public class ChangeStatusEntry : ScenarioEntry
    {
        public ChangeStatusDetail event_detail;

        public struct ChangeStatusDetail
        {
            public string host_name;
            public string host_status;
        }
    }
}
