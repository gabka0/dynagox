param(
  [Parameter(Mandatory = $true)][string]$Dataset,
  [string]$OutCsv = "dynagox_cpp/implement/bench_results.csv",
  [int]$Seed = 42,
  [int]$UpdateCount = 200000,
  [int]$MilestoneEvery = 20000,
  [double]$InitialRatio = 0.5
)

$exe = "dynagox_cpp/implement/build/experiments/Debug/bench_linek_count.exe"
if (!(Test-Path $exe)) {
  Write-Error "bench_linek_count.exe not found. Build it first."
  exit 1
}
if (!(Test-Path $Dataset)) {
  Write-Error "Dataset not found: $Dataset"
  exit 1
}

Remove-Item $OutCsv -ErrorAction Ignore

$headerDone = $false
for ($k = 2; $k -le 6; $k++) {
  for ($a = 1; $a -le ($k + 1); $a++) {
    for ($b = $a; $b -le [Math]::Min($k + 1, $a + 4); $b++) {
      $args = @(
        "--dataset", $Dataset,
        "--k", $k,
        "--a", $a,
        "--b", $b,
        "--update-count", $UpdateCount,
        "--milestone-every", $MilestoneEvery,
        "--seed", $Seed,
        "--initial-ratio", $InitialRatio
      )
      if (-not $headerDone) {
        $args += "--csv-header"
        $headerDone = $true
      }
      & $exe @args | Add-Content $OutCsv
    }
  }
}

Write-Host "Done: $OutCsv"
