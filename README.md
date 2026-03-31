# HPC Genomic Pipeline: JL Random Projection + MLP Autoencoder for SNP Data

A high-performance computing pipeline for genomic dimensionality reduction that replaces traditional PCA with a **Johnson-Lindenstrauss (JL) Random Projection** followed by an **MLP Autoencoder**, achieving massive speed gains with minimal accuracy trade-off.

> **Hardware:** NVIDIA GeForce RTX 3060 Laptop GPU (6 GB VRAM) · CUDA + AMP

---

## Pipeline Architecture

```
Raw SNP CSV (16,735 SNPs × 150 samples)
   ↓  Additive Encoding via custom C extension (ctypes)
Numeric Matrix  (150 × 16,735)
   ↓  Johnson-Lindenstrauss Random Projection
JL-Projected Matrix  (150 × 128)
   ↓  MLP Autoencoder (GPU + Mixed Precision)
Latent Embeddings + Reconstruction
```

## HPC Components

| Component | Implementation | Benefit |
|:---|:---|:---|
| **Native Code** | Custom C SNP encoder via `ctypes` | Eliminates Python interpreter overhead |
| **GPU Parallelism** | PyTorch CUDA (3,584 cores) | Thousands of parallel matrix operations |
| **Mixed Precision** | `torch.cuda.amp` (FP16/FP32) | ~2× compute throughput, reduced VRAM |
| **Algorithmic Efficiency** | JL Projection vs PCA | O(n·d·k) vs O(d³) complexity |
| **Regularization** | BatchNorm + GELU + Dropout + Weight Decay + Early Stopping + Cosine LR | Stable convergence on small datasets |

## Key Results

| Metric | Value |
|:---|:---|
| C vs Python encoding speedup | **~2.8×** |
| K-Fold Mean RMSE (5-fold CV) | **0.921 ± 0.031** |
| Final Test RMSE (JL + MLP) | **0.936** |
| JL-only baseline RMSE | **78.636** |
| MLP improvement over JL-only | **98.81%** |

## Repository Structure

```
├── JL_MLP_SNP_Pipeline.ipynb   # Full annotated pipeline notebook
├── scripts/
│   ├── encode_utils.c           # Custom C SNP encoding extension
│   └── setup.py                 # Build script for C extension
├── data/
│   ├── final_results.csv        # Test set evaluation results
│   └── kfold_summary.csv        # K-Fold cross-validation summary
├── outputs/
│   └── images/                  # All generated visualizations
├── requirements.txt
├── .gitignore
└── README.md
```

> **Note:** The raw dataset (`data_geno_hmp.csv`, ~8 MB) is excluded from this repository.
> It is a standard HapMap-format SNP genotype file with 16,735 SNPs × 150 samples.

## How to Run

```bash
# 1. Install dependencies
pip install -r requirements.txt

# 2. Build the C extension (requires a C compiler)
cd scripts && python setup.py build_ext --inplace && cd ..

# 3. Place your HapMap CSV at data/data_geno_hmp.csv

# 4. Run the notebook
jupyter notebook JL_MLP_SNP_Pipeline.ipynb
```

## Model Architecture

```
SNPAutoencoder (8,983,263 trainable parameters)
  Encoder: 128 → 512 → 256 → 128  (BatchNorm + GELU + Dropout at each layer)
  Decoder: 128 → 256 → 512 → 16,735  (mirrors encoder)
  Init: Kaiming Normal
  Optimizer: AdamW (lr=1e-3, weight_decay=1e-4)
  Scheduler: CosineAnnealingLR
  Loss: MSELoss
```

## Visualizations

The notebook generates the following plots (saved in `outputs/images/`):

- **C vs Python Speedup** — Bar chart comparing encoding times
- **EDA Plots** — MAF distribution, mean dosage, SNP heatmap
- **JL Projection Analysis** — Distance preservation & PCA variance comparison
- **CPU vs GPU Timing** — Profiling JL and PCA on CPU vs GPU
- **K-Fold Training Curves** — Loss and RMSE per fold
- **Reconstruction Comparison** — JL-only vs MLP+JL bar chart
- **Latent Space Visualization** — t-SNE of learned embeddings
- **PCA vs JL+MLP Tradeoff** — Speed/accuracy comparison chart

## License

This project is for academic/educational purposes.
