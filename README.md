# ZMK Module for BMP Boost

To include this module in your ZMK project, add the following entries.

Sample: [zmk-bmp-boost-sample](https://github.com/sekigon-gonnoc/zmk-bmp-boost-sample)

## west.yaml

```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    - name: sekigon-gonnoc
      url-base: https://github.com/sekigon-gonnoc
  projects:
    - name: zmk-component-bmp-boost
      remote: sekigon-gonnoc
      import: west.yml
```

## build.yaml

Please adjust the shield name to match your own keyboard.

```yaml
include:
  - board: bmp_boost
    shield: tester_pro_micro
```

## config

``` bash
# For coin cell
CONFIG_ZMK_NON_LIPO_MIN_MV=2000
CONFIG_ZMK_NON_LIPO_MAX_MV=3000
CONFIG_ZMK_NON_LIPO_LOW_MV=0

# For Alkaline battery(1S)
CONFIG_ZMK_NON_LIPO_MIN_MV=1500
CONFIG_ZMK_NON_LIPO_MAX_MV=900
CONFIG_ZMK_NON_LIPO_LOW_MV=0

# For Ni-MH battery(1S)
# The default is NiMH(1S), so no changes are needed.
```