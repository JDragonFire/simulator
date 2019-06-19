using System.Reflection;
using System.Text;

namespace ScenarioPreprocessor
{
    public class StaticHostInfo
    {
        #region Equality members

        public bool EqualsExceptMemory(StaticHostInfo other)
        {
            return CPU_FACTOR == other.CPU_FACTOR &&
                   NCPUS == other.NCPUS &&
                   NPROCS == other.NPROCS &&
                   NCORES == other.NCORES &&
                   NTHREADS == other.NTHREADS &&
                   MAX_SLOT == other.MAX_SLOT;
        }

        public static StaticHostInfo Compare(StaticHostInfo a, StaticHostInfo b)
        {
            if (a == b)
                return a;

            int compare;
            compare = a.MAX_MEM.CompareTo(b.MAX_MEM);
            if (compare > 0)
                return a; 
            if (compare <0)
                return b;
            compare = a.MAX_SLOT.CompareTo(b.MAX_SLOT);
            if (compare > 0)
                return a; 
            if (compare <0)
                return b;
            compare = a.MAX_SWP.CompareTo(b.MAX_SWP);
            if (compare > 0)
                return a; 
            if (compare <0)
                return b;
            compare = a.MAX_TEP.CompareTo(b.MAX_TEP);
            if (compare > 0)
                return a; 
            if (compare <0)
                return b;
            compare = a.NTHREADS.CompareTo(b.NTHREADS);
            if (compare > 0)
                return a; 
            if (compare <0)
                return b;

            return a;
        }

        protected bool Equals(StaticHostInfo other)
        {
            return CPU_FACTOR.Equals(other.CPU_FACTOR) && NCPUS == other.NCPUS && NPROCS == other.NPROCS && NCORES == other.NCORES && NTHREADS == other.NTHREADS && MAX_SLOT == other.MAX_SLOT && MAX_MEM == other.MAX_MEM && MAX_SWP == other.MAX_SWP && MAX_TEP == other.MAX_TEP;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((StaticHostInfo) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                int hashCode = CPU_FACTOR.GetHashCode();
                hashCode = (hashCode * 397) ^ NCPUS;
                hashCode = (hashCode * 397) ^ NPROCS;
                hashCode = (hashCode * 397) ^ NCORES;
                hashCode = (hashCode * 397) ^ NTHREADS;
                hashCode = (hashCode * 397) ^ MAX_SLOT;
                hashCode = (hashCode * 397) ^ MAX_MEM;
                hashCode = (hashCode * 397) ^ MAX_SWP;
                hashCode = (hashCode * 397) ^ MAX_TEP;
                return hashCode;
            }
        }

        public static bool operator ==(StaticHostInfo left, StaticHostInfo right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(StaticHostInfo left, StaticHostInfo right)
        {
            return !Equals(left, right);
        }

        #endregion

        public float CPU_FACTOR;
        public int NCPUS;
        public int NPROCS;
        public int NCORES;
        public int NTHREADS;
        public int MAX_SLOT;
        public int MAX_MEM;
        public int MAX_SWP;
        public int MAX_TEP;
        public string STATUS;

        public static StaticHostInfo FromRaw(RptHardwareRawEntry entry)
        {
            return new StaticHostInfo
            {
                CPU_FACTOR = entry.CPU_FACTOR,
                NCPUS = entry.NCPUS,
                NPROCS = entry.NPROCS,
                NCORES = entry.NCORES,
                NTHREADS = entry.NTHREADS,
                MAX_SLOT = entry.MAX_SLOT,
                MAX_MEM = (int) entry.MAX_MEM,
                MAX_SWP = (int) entry.MAX_SWP,
                MAX_TEP = (int) entry.MAX_TMP
            };
        }

        public override string ToString()
        {
            var sb = new StringBuilder();
            foreach (FieldInfo field in typeof(StaticHostInfo).GetFields())
                sb.AppendLine($"# {field.Name} : {field.GetValue(this)}");
            return sb.ToString();
        }
    }
}