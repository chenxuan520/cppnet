# cppnet::File
## static int Read(const std::string &path, std::string &data)
- 参数：
    - `path`：文件路径字符串。
    - `data`：用于存储读取到的文件数据的字符串引用。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：读取指定文件的内容并存储到 `data` 中。
## static int Write(const std::string &path, const std::string &data)
- 参数：
    - `path`：文件路径字符串。
    - `data`：要写入文件的内容字符串。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：将 `data` 的内容写入指定文件。
## static bool Exist(const std::string &path)
- 参数：
    - `path`：文件路径字符串。
- 返回值：`bool`类型，文件存在返回 `true`，不存在返回 `false`。
- 作用：检查指定文件是否存在。注意：如果是目录则返回 `false`。
## static std::string Suffix(const std::string &path)
- 参数：
    - `path`：文件路径字符串。
- 返回值：`std::string`类型，文件后缀名（不包括点）。
- 作用：获取指定文件的后缀名。
## static int Create(const std::string &path)
- 参数：
    - `path`：文件路径字符串。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：创建指定文件，如果路径中的目录不存在则自动创建。
## static int Remove(const std::string &path)
- 参数：
    - `path`：文件路径字符串。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：删除指定文件。
## static int Append(const std::string &path, const std::string &data)
- 参数：
    - `path`：文件路径字符串。
    - `data`：要追加到文件的内容字符串。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：将 `data` 的内容追加到指定文件末尾。
## static bool CanWrite(const std::string &path)
- 参数：
    - `path`：文件路径字符串。
- 返回值：`bool`类型，文件可写返回 `true`，不可写返回 `false`。
- 作用：检查指定文件是否具有写入权限。
## static int LineCount(const std::string &path)
- 参数：
    - `path`：文件路径字符串。
- 返回值：`int`类型，文件的行数。如果文件不存在则返回 0。
- 作用：获取指定文件的行数。
## static bool IsDir(const std::string &path)
- 参数：
    - `path`：文件路径字符串。
- 返回值：`bool`类型，是目录返回 `true`，不是目录返回 `false`。
- 作用：判断指定路径是否是一个目录。
## static bool IsFile(const std::string &path)
- 参数：
    - `path`：文件路径字符串。
- 返回值：`bool`类型，是文件返回 `true`，不是文件返回 `false`。
- 作用：判断指定路径是否是一个文件。
