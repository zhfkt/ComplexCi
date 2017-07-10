package org.dc.algorithm

import org.apache.log4j.Logger
import org.dc.Judge
import org.dc.result.Result

import java.util.zip.ZipEntry
import java.util.zip.ZipFile

/**
 * Created by jay on 2017/6/13.
 * 大师赛计算
 */
class NetMaster implements Judge{

    public static void main(String[] args) {

    if(args.length < 2)
    {

        println " groovy  -cp 'target/algorithm-1.0-SNAPSHOT.jar:/root/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar'  src/main/java/org/dc/algorithm/NetMaster.groovy [csv] [zip]"

        return;

    }

        println new NetMaster().judge(args[0],args[1]);
    }

    /**
     * 格式检查在另外一个方法里面进行，这里默认就是正确格式
     * @param produceFile
     * @param judgeFile
     * @return
     */
    @Override
    Result judge(String produceFile, String judgeFile) {
        //先对network解压,可以缓存一次。。。。/tmp/tomcat-docbase.1483434272471594753.10000/competition_data/testSet/networks.zip
        File tempFile = new File(judgeFile)
        if (!new File(tempFile.getParent()+"/networks/model1.csv").isFile()){
            unZipFiles(tempFile,tempFile.getParent()+"/")
        }
        judgeFile = tempFile.getParent()+"/networks/"
        Result result = new Result();
        result.put(0,0D);
        println("begin compute");
        File oldfile = new File(produceFile);
        if (!oldfile.exists()) {
            result.setMsg("结果文件不存在");
            return result;
        }
        def map = ["model1":1039722,
                   "model2":1083568,
                   "model3":997663,
                   "model4":1001733,
                   "real1":1694616,
                   "real2":1957027,
                   "real3":426485,
                   "real4":855802]

        println("检查数据格式");
        def results =rule(produceFile,map)
        println("完成数据加载");
        if (!results.flag){
            println results.msg
            result.setMsg(results['msg'].toString())
            return result
        }


        def produce = results.data as HashMap
        Double sum = 0D
        map.keySet().each {
            if (produce.containsKey(it)){//包含这个结点
                Double eachScore = computeResultWithIndex(produce.get(it),judgeFile+"${it}.csv")
                println("${it}:"+ eachScore)
                sum+=eachScore
            }else{
                sum+=1
            }

        }
        println(sum)
        result.put(0,sum);
        result.status = true
        return result
    }

    /**
     * 检测输入文件的格式
     * @param produceFile
     * @return
     */
    def rule(String produceFile,Map map){
        def resultMap = new HashMap()
        def file = new File(produceFile)
        Map<String,LinkedHashSet<Integer>> result = new HashMap<String,LinkedHashSet<Integer>>()
        int i = 0
        try {
            file.eachLine{
                i++
                if (i!=1){//第一行不要处理
                    def temp = it.split(",")
                    if (!map.containsKey(temp[0])){
                        throw new Exception("第${i}行,没有正确的网络ID")
                    }
                    if (temp.size()>=2){
                        for (int j = 1; j < temp.length; j++) {
                            if (!temp[j].isNumber()){
                                throw new Exception("第${i}行,含有非法字符")
                            }
                            if (result.get(temp[0])){
                                def set = result.get(temp[0])
                                set.add(temp[j].toInteger())
                                result.put(temp[0],set)
                            }else{
                                def set = new LinkedHashSet<Integer>()
                                set.add(temp[j].toInteger())
                                result.put(temp[0], set)
                            }
                        }
                    }
                }
            }
        }catch (Exception e){
            resultMap.flag=false
            resultMap.msg = e.getMessage()
            return resultMap
        }


        //提交的点数错误问题
        def msg = new StringBuilder("")
        map.each {
            if (result.containsKey(it.key)){
                if (it.value!=result.get(it.key).size()){
                    println("error")
                    msg.append("${it.key}提交的点数错误,要么提交所有点，要么都不提交\n")
                }
            }
        }
        if (!msg.toString().equals("")){
            resultMap.flag=false
            resultMap.msg=msg
            return resultMap
        }


        resultMap.flag=true
        resultMap.data=result
        return resultMap
    }


    /**
     * 计算最大联通集团,双向边,增加了每条边的索引位置
     * @param produce 输入文件
     * @param judge 输出文件
     * @return
     */
    def computeResultWithIndex(LinkedHashSet<Integer> produce,String judge){
        def map = new HashMap<Integer,ArrayList<Integer>>()
        def produceList = new LinkedList<Integer>(produce)
        def file = new File(judge)
        Long tt = System.currentTimeMillis()
        println("开始构建网络")
        file.eachLine {
            def temp = it.split(",")
            Integer temp0 = temp[0].toInteger()
            Integer temp1 = temp[1].toInteger()
            def set = map.get(temp0)?:new ArrayList<>()
            set.add(temp1)
            map.put(temp0,set)
            set = map.get(temp1)?:new ArrayList<>()
            set.add(temp0)
            map.put(temp1,set)
        }
        println(System.currentTimeMillis()-tt)

        def sumList = new HashMap<Integer,HashSet<Integer>>()//存储所有的节点
        def vMap = new HashMap<Integer,Integer>()//存储所有节点的位置
        Double sum = 0D //当前最大的连通节点数量
        Double sumNum = 0D //所有连通节点的总数量
        println("开始建图")

        int index_v = 0
        produceList.reverseEach {//反向循环建立边集合点
            index_v++
            def temp = map.get(it) as ArrayList<Integer>//得到这个节点所有的边
            if (temp){//占有边

                def vSet = null as HashSet
                def key = null
                boolean  flag = false
                temp.each {v->
                    if (vMap.containsKey(v)){
                        if (!vSet){
                            key = vMap.get(v)//第一个节点的索引ID //219733
                            vSet = sumList.get(key)
                        }else{
                            def s_vSet = sumList.get(vMap.get(v))
                            if (s_vSet.size()>vSet.size()){
                                vSet = s_vSet
                                key = vMap.get(v)
                            }
                        }
                    }
                }
                if (vSet){
                    vSet.add(it)
                    //增加占用索引
                    vMap.put(it,key)
                    temp.each {v->//2,3,4,6,7。。如果找到能购被合并,//todo 顺序
                        if (vMap.containsKey(v)){//这个节点已经被使用，那么合并这个节点//v-1031029
                            flag = true
                            def s_vSet = sumList.get(vMap.get(v))
                            if (vSet!=s_vSet){
                                 vSet.addAll(s_vSet)
                                sumList.remove(vMap.get(v))//删除被合并的集团,先删除
                                    s_vSet.each {v_it->
                                        vMap.put(v_it,key)
                                    }
                                }
                        }
                    }
                }
                if (vSet){
                    if (vSet.size()>sumNum) sumNum = vSet.size()
                }
                if (!flag){//没有能够被合并的点，新建集合点
                    HashSet set = new HashSet()
                    set.add(it)
                    sumList.put(index_v,set)
                    vMap.put(it,index_v)
                    if (set.size()>sumNum) sumNum = set.size()
                }
            }else{//孤独的点
                print("error")
            }
            sum+=sumNum
        }
        println("总数：${sum-produceList.size()}")
        return (sum-produceList.size())/produceList.size()/produceList.size()
    }

    static void unZipFiles(File zipFile,String descDir)throws IOException{
        File pathFile = new File(descDir);
        if(!pathFile.exists()){
            pathFile.mkdirs();
        }
        ZipFile zip = new ZipFile(zipFile);
        for(Enumeration entries = zip.entries();entries.hasMoreElements();){
            ZipEntry entry = (ZipEntry)entries.nextElement();
            String zipEntryName = entry.getName();
            InputStream ins = zip.getInputStream(entry);
            String outPath = (descDir+zipEntryName).replaceAll("\\*", "/");;
            //判断路径是否存在,不存在则创建文件路径
            File file = new File(outPath.substring(0, outPath.lastIndexOf('/')));
            if(!file.exists()){
                file.mkdirs();
            }
            //判断文件全路径是否为文件夹,如果是上面已经上传,不需要解压
            if(new File(outPath).isDirectory()){
                continue;
            }
            //输出文件路径信息
            println(outPath);

            OutputStream out = new FileOutputStream(outPath);
            byte[] buf1 = new byte[1024];
            int len;
            while((len=ins.read(buf1))>0){
                out.write(buf1,0,len);
            }
            ins.close();
            out.close();
        }
        println("******************解压完毕********************");
    }
}
