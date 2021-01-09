from ac_diskstructs import HashMap


def test_HashMap_impl(test_dir, non_existent_key, data, more_data, key_type=None):
    path = test_dir.file()
    bucket_count = len(data) * 2 + len(more_data) * 2

    def check_HashMap_contents(hm, obj):
        for (key, value) in obj:
            assert value.encode('utf-8') == hm.get(key), f'Value of {key} must be the same'

        assert hm.get(non_existent_key) is None, 'Non-existent key must not be present'

    def check_HashMap_bucket_count(hm):
        assert hm.bucket_count() == bucket_count, 'Bucket count must be the same'

    def create_HashMap():
        hm = HashMap(path, key_type=key_type, bucket_count=bucket_count)

        check_HashMap_bucket_count(hm)

        for (key, value) in data:
            hm[key] = value

        check_HashMap_contents(hm, data)

        hm.close()

        check_HashMap_contents(hm, data)

        hm.delete()

    def update_HashMap():
        hm = HashMap(path, key_type=key_type, rw=True)

        check_HashMap_bucket_count(hm)
        check_HashMap_contents(hm, data)

        for (key, value) in more_data:
            hm[key] = value

        check_HashMap_contents(hm, more_data)

        hm.close()

        check_HashMap_contents(hm, data)
        check_HashMap_contents(hm, more_data)

        hm.delete()

    def read_HashMap():
        hm = HashMap(path, key_type=key_type)

        check_HashMap_bucket_count(hm)
        check_HashMap_contents(hm, data)
        check_HashMap_contents(hm, more_data)

        all = {}

        for (key, value) in hm.items():
            if key_type == bytes:
                key = key.decode('utf-8')

            all[key] = value.decode('utf-8')

        for obj in [data, more_data]:
            for (key, value) in obj:
                assert all.get(key) == value, f'Value of {key} must be the same'

                del all[key]

        assert len(all) == 0, 'No keys are lost, no extra keys'

    create_HashMap()
    update_HashMap()
    read_HashMap()


def test_HashMap(test_dir):
    test_HashMap_impl(
        test_dir,
        1337,
        [
            [1, "value"],
            [2, "\0\1\2\3\0\4\5\6"],
            [3, "third value"],
        ],
        [
            [4, '''Lorem ipsum dolor sit amet, consectetur adipiscing
elit, sed do eiusmod tempor incididunt ut labore et
dolore magna aliqua. Ut enim ad minim veniam, quis'''],
            [5, '''nostrud exercitation ullamco laboris nisi ut aliquip
ex ea commodo consequat. Duis aute irure dolor in
reprehenderit in voluptate velit esse cillum dolore'''],
        ],
    )

    test_HashMap_impl(
        test_dir,
        "non-existent key",
        [
            ["first key", "value"],
            ["second key", "other value"],
            ['''Lorem ipsum dolor sit amet, consectetur adipiscing
elit, sed do eiusmod tempor incididunt ut labore et
dolore magna aliqua. Ut enim ad minim veniam, quis''', "third value"],
        ],
        [
            ["\0\0\0\0\0\1\2\4", "\6\1\6\7\245\0\46"],
            ["key", '''nostrud exercitation ullamco laboris nisi ut aliquip
ex ea commodo consequat. Duis aute irure dolor in
reprehenderit in voluptate velit esse cillum dolore'''],
        ],
        bytes,
    )
