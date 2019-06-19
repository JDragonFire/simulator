using System;

namespace ScenarioPreprocessor
{
    public class RptHardwareRawEntry
    {
        /// <summary>
        /// The name of the LSF cluster.
        /// </summary>
        public string CLUSTER_NAME { get; set; }
        /// <summary>
        /// The time that the sample is taken.
        /// </summary>
        public DateTime TIME_STAMP { get; set; }
        /// <summary>
        /// In the format `TO_CHAR`
        /// </summary>
        public string ISO_WEEK { get; set; }

        public int TIME_STAMP_GMT { get; set; }

        public string HOST_NAME { get; set; }

        public string CLUSTER_HOST { get; set; }
        /// <summary>
        /// Type of host you have, such as LINUX86.
        /// </summary>
        public string LSFHOST_TYPE { get; set; }
        /// <summary>
        /// The host model of the most, such as UltraSparc10.
        /// </summary>
        public string LSFHOST_MODEL { get; set; }
        /// <summary>
        /// Speed of the host's CPU relative to other hosts in the cluster.
        /// If one processor is twice the speed of another, its CPU factor should be twice as large.
        /// The CPU factors are defined by the administrator.
        /// For multiprocessor hosts, the CPU factor is the speed of a single processor;
        /// the system automatically scales the host CPU load to account for additional processors.
        /// </summary>
        public float CPU_FACTOR { get; set; }

        public int NCPUS { get; set; }

        public int NPROCS { get; set; }

        public int NCORES { get; set; }

        public int NTHREADS { get; set; }

        public string HOST_GROUP { get; set; }

        public string HOST_STATUS { get; set; }

        public int MAX_SLOT { get; set; }

        public int RUN_SLOT { get; set; }
        /// <summary>
        /// Number of current users logged on to the system.
        /// </summary>
        public int LS { get; set; }
        /// <summary>
        /// Amount of time in minutes that a host has been idle.
        /// On a Linux/UNIX host, it is the amount of time since the keyboard has been touched on
        /// all logged in sessions.
        /// on a Windows host, it is the amount of time a screen saver has been active.
        /// </summary>
        public int IT { get; set; }
        /// <summary>
        /// Load this host carries, averaged over the last 15 minutes.
        /// The load is the average number of processes using the CPU during a given time interval.
        /// </summary>
        public double R15M { get; set; }
        public double R15S { get; set; }
        public double R1M { get; set; }
        /// <summary>
        /// Current CPU utilization of your host, as a percentage.
        /// </summary>
        public double UT { get; set; }
        /// <summary>
        /// I/O throughput to disk
        /// </summary>
        public int IO { get; set; }
        /// <summary>
        /// Estimate of the real memory, in MB, currently available to user processes.
        /// This represents the approximate size of the largest process that could
        /// be started on a host without causing the host to start paging.
        /// </summary>
        public long MEM { get; set; }

        public long SWP { get; set; }

        public long TMP { get; set; }

        public long MAX_MEM { get; set; }

        public long MAX_SWP { get; set; }

        public long MAX_TMP { get; set; }
        /// <summary>
        /// Virtual memory paging rate in pages per second.
        /// This index is closely tied to the amount of available memory
        /// and the total size of the processes running on a host;
        /// if there is not enough memory to satisfy all processes, the
        /// paging rate is high.
        /// </summary>
        public double PG { get; set; }

        public int RESOURCE_METRICS_INTERVAL { get; set; }

        public int LSF_BHOSTS_INTERVAL { get; set; }

        public string CLUSTER_MAPPING { get; set; }
    }
}