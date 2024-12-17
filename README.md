
Oyun Yapısı:
Bu oyun bir Dungeon Adventure oyunudur ve aşağıdaki temel bileşenlerden oluşur:
Oyuncu (Player):
Oyuncunun sağlık (health), güç (strength), zırh (armor) gibi özellikleri vardır.
Oyuncunun en fazla 5 eşya taşıyabileceği bir envanteri (inventory) vardır.
Oyuncu bir odadan diğerine geçiş yapabilir ve düşmanlarla savaşabilir.
Oda (Room):
Oyun odalardan oluşan bir haritada oynanır.
Her oda bir açıklamaya (description) ve 4 farklı yön bağlantısına (up, down, left, right) sahiptir.
Her odada eşyalar (items) ve düşmanlar (Enemy) bulunabilir.
Eşyalar (Items):
Oyuncu odalardan eşya toplayabilir ve envanterine ekleyebilir.
Örnek eşyalar: Kılıç (Sword), Zırh (Armor), Meşale (Torch).
Düşmanlar (Enemies):
Odalarda düşmanlar bulunur. Örnek: Kurtadam (Werewolf), Büyük Örümcek (Huge Spider).
Oyuncu, düşmanlara saldırarak onları yenebilir.
   Komutlar:
       Oyun başladığında, oyuncu başlangıç odasında olur.
       Oyuncu komutlar (keywords) yazarak oyunu oynar. Örneğin:
"move directionname" → Bir üst odaya geçiş yapar.
"look" → Odadaki detayları ve varlıkları görüntüler.
"pickup item_name" → Odadaki bir eşyayı alır.
"attack" → Odadaki düşmana saldırır.
"save filename" → Oyunu kaydeder.
"load filename" → Oyunu yükler
Oyuncunun amacı, odaları keşfetmek, düşmanlarla savaşmak ve hayatta kalmaktır.
Metodlar ve Çalışma Şemaları:
1. start(Player *player, Room *rooms)
Oyuncu ve odalar başlatılır.
Yapılan işlemler:
Tüm odalar tanımlanır ve birbirine bağlanır (link_rooms ile).
Eşyalar ve düşmanlar odalara yerleştirilir.
Oyuncunun sağlık, zırh ve başlangıç özellikleri atanır.
2. player_move(char *dir, Player *player)
Oyuncunun seçtiği yöne hareket etmesini sağlar.
İşlem Akışı:
Yön (up, down, left, right) kontrol edilir.
Oyuncunun gidebileceği bir oda varsa, oyuncu yeni odaya taşınır.
Yeni oda görüntülenir (look metodu çağrılır).
3. look(Player *player)
Oyuncunun bulunduğu odanın detaylarını gösterir.
Yapılan İşlemler:
Odadaki açıklama yazdırılır.
Mevcut eşyalar listelenir.
Odada bir düşman varsa, düşmanın adı ve sağlığı gösterilir.
4. pickup_item(Player *player, char *item_name)
Oyuncunun odadaki bir eşyayı almasını sağlar.
İşlem Akışı:
Oyuncunun envanteri kontrol edilir (doluysa uyarı verir).
Odadaki eşyalar taranır ve isim eşleşmesi yapılır.
Eşleşen eşya bulunursa, envantere eklenir ve odadan kaldırılır.
Eğer zırh alınırsa, oyuncunun zırh değeri artırılır.
5. attack(Player *player) ve fight(Player *player, Enemy *Enemy)
Düşmanla savaşma işlemi gerçekleştirilir.
İşlem Akışı:
Eğer odada düşman yoksa, uyarı verir.
Oyuncu, gücüne göre düşmana zarar verir.
Düşman, gücüne göre oyuncuya zarar verir.
Dövüş, biri yenilene kadar devam eder.
6. save_game(Player *player, const char *filename) ve load_game(Player *player, const char *filename)
Oyunun durumunu kaydeder ve yükler.
Save İşlemi:
Oyuncunun sağlık, güç, zırh, ve envanter bilgileri dosyaya yazılır.
Load İşlemi:
Dosyadan oyuncunun bilgileri okunur ve oyuna yüklenir.
Anahtar Özellikler:
Dinamik Bellek Yönetimi: malloc kullanılarak odalar, eşyalar ve düşmanlar dinamik olarak yaratılır.
Oda Bağlantıları: link_rooms ile odalar 4 yönlü olarak birbirine bağlanır.
Envanter Yönetimi: Oyuncunun en fazla 5 eşya taşımasına izin veren bir sistem tasarlanmıştır.
Dövüş Mekaniği: Sıra tabanlı dövüş sistemiyle oyuncu ve düşman sırayla saldırır.

Oyun Haritası Örneği:
Oda yapısı bir + sembolü şeklindedir:
       [1]
        |
  [3] - [0] - [4]
        |
       [2]

[0]: Başlangıç odası
[1]: Üst kat
[2]: Bodrum
[3]: Sol oda
[4]: Sağ oda



