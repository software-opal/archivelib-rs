from archivelib.compress.byte_run_hash_table import ByteRunHashTable

def test_hash_table():
    table = ByteRunHashTable.new(1 << 10)

    # 'abc'
    table.record_byte(97)
    table.record_byte(98)
    table.record_byte(99)

    assert table.current_hash == 2883
