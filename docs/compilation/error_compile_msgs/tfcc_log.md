# Error Logs

In the building of the Tensorflow C++ API, a lot of errors were being spit out that where difficult to fully understand, yet were saved in the hopes to try and make better sense of them on top of "saving work".

## 09/10/2022

```bash
ERROR: /home/goose/Source/tensorflow/tensorflow/python/keras/api/BUILD:133:19: Executing genrule //tensorflow/python/keras/api:keras_python_api_gen_compat_v1 failed (Exit 1): bash failed: error executing command /bin/bash -c ... (remaining 1 argument(s) skipped)
Traceback (most recent call last):
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/tools/api/generator/create_python_api.py", line 26, in <module>
    from tensorflow.python.tools.api.generator import doc_srcs
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/__init__.py", line 49, in <module>
    from tensorflow.python.feature_column import feature_column_lib as feature_column
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/feature_column/feature_column_lib.py", line 22, in <module>
    from tensorflow.python.feature_column.feature_column import *
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/feature_column/feature_column.py", line 147, in <module>
    from tensorflow.python.layers import base
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/layers/base.py", line 20, in <module>
    from tensorflow.python.keras.legacy_tf_layers import base
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/keras/__init__.py", line 25, in <module>
    from tensorflow.python.keras import models
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/keras/models.py", line 20, in <module>
    from tensorflow.python.keras import metrics as metrics_module
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/keras/metrics.py", line 34, in <module>
    from tensorflow.python.keras import activations
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/keras/activations.py", line 18, in <module>
    from tensorflow.python.keras.layers import advanced_activations
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/keras/layers/__init__.py", line 29, in <module>
    from tensorflow.python.keras.layers.preprocessing.image_preprocessing import CenterCrop
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/keras/layers/preprocessing/image_preprocessing.py", line 28, in <module>
    from tensorflow.python.keras.preprocessing import image as image_preprocessing
  File "/home/goose/.cache/bazel/_bazel_goose/26e8467db4a0dbc84479e69c49ed8c9f/execroot/org_tensorflow/bazel-out/k8-opt/bin/tensorflow/python/keras/api/create_tensorflow.python_api_keras_python_api_gen_compat_v1.runfiles/org_tensorflow/tensorflow/python/keras/preprocessing/__init__.py", line 19, in <module>
    import keras_preprocessing
ModuleNotFoundError: No module named 'keras_preprocessing'
ERROR: /home/goose/Source/tensorflow/tensorflow/lite/python/BUILD:63:10 Executing genrule //tensorflow/python/keras/api:keras_python_api_gen_compat_v1 failed (Exit 1): bash failed: error executing command /bin/bash -c ... (remaining 1 argument(s) skipped)
INFO: Elapsed time: 2821.598s, Critical Path: 120.86s
INFO: 10913 processes: 1302 internal, 9611 local.
FAILED: Build did NOT complete successfully
```

