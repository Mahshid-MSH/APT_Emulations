# APT Emulations for MITRE Caldera

Welcome to my APT Emulations repository! This project is a growing, community-driven collection of adversary emulation plans designed specifically for **MITRE Caldera**. 

I am currently studying adversary emulation, threat modeling, and purple teaming. I built these profiles as a safe way to translate real-world Cyber Threat Intelligence (CTI) into actionable Caldera campaigns. All payloads used in these plans are strictly **benign** (e.g., executing `echo`, using `127.0.0.1`, and leveraging standard native binaries), making them completely safe to execute in your home lab or corporate defensive environments.

## Available Emulations

**Lazarus Group** | [Operation In(ter)ception (June 2020)](Lazarus/Lazarus_Operation_Interception) | `Lazarus/Lazarus_Operation_Interception`
*(More coming soon)* 

---

## How to Use (Minimal Effort Installation)

The repository is structured to mirror Caldera's backend. This means you do not need to map YAML files manually—you just pick the APT you want and copy the folder over!

### Step 1: Clone the Repository
```bash
git clone https://github.com/Mahshid-MSH/APT_Emulations.git
cd APT_Emulations
```

### Step 2: Choose Your APT
Navigate to the specific campaign you want to emulate. For example, to load the Lazarus campaign:

```bash
cd Lazarus/Lazarus_Operation_Interception
```

### Step 3: Copy to Caldera
Copy the contents of the local data/ folder directly into your Caldera emu (or stockpile) plugin directory.
(Note: Adjust /path/to/caldera/ to match where Caldera is installed on your machine).

```bash
cp -r data/* /path/to/caldera/plugins/emu/data/
```

### Step 4: Restart and Run
Restart your Caldera server so it can parse the newly added UUIDs and YAML configurations.

Log into the Caldera Web UI.

Navigate to Campaigns ➔ Adversaries.

Select the new APT profile from the dropdown (e.g., Lazarus - Operation In(ter)ception).

Build an operation and run it against your agents!


🏗️ Repository Architecture
To keep things organized as this repository grows, everything is sorted by Threat Actor ➔ Campaign Name. Inside each campaign is a data folder that perfectly matches Caldera's plugin structure.

```plaintext
.
├── README.md
└── Lazarus/
    └── Lazarus_Operation_Interception/
        └── data/
            ├── abilities/     <-- Contains the individual TTPs (YAML)
            └── adversaries/   <-- Contains the main attack flow (YAML)
```

🤝 Let's Learn Together (Contributing)
I am actively learning, and I would love your help!

Whether you are a seasoned red teamer, a SOC analyst, or a fellow student, contributions are highly encouraged. If you spot a mistake, know a better way to write a Caldera ability, or want to add an entirely new APT emulation plan to the repo, please jump in.
